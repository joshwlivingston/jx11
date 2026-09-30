#include "LookAndFeel.h"
#include "Skin.h"
#include "Textures.h"

LookAndFeel::LookAndFeel() {
  setColour(juce::ResizableWindow::backgroundColourId, Skin::aluminiumDark);
  setColour(juce::Label::textColourId, Skin::inkOnMetal);

  // Slider values are letterpress text on the metal, not boxes.
  setColour(juce::Slider::textBoxTextColourId, Skin::inkOnMetal);
  setColour(juce::Slider::textBoxBackgroundColourId,
            juce::Colours::transparentBlack);
  setColour(juce::Slider::textBoxOutlineColourId,
            juce::Colours::transparentBlack);
  setColour(juce::Slider::textBoxHighlightColourId,
            Skin::blue.withAlpha(0.35f));

  // Double-clicking a value opens an iOS text field.
  setColour(juce::Label::textWhenEditingColourId, juce::Colours::black);
  setColour(juce::Label::backgroundWhenEditingColourId, juce::Colours::white);
  setColour(juce::Label::outlineWhenEditingColourId, Skin::blue);
  setColour(juce::TextEditor::textColourId, juce::Colours::black);
  setColour(juce::TextEditor::highlightColourId, Skin::blue.withAlpha(0.35f));
  setColour(juce::TextEditor::highlightedTextColourId, juce::Colours::black);
  setColour(juce::CaretComponent::caretColourId, Skin::blue);

  setColour(juce::PopupMenu::backgroundColourId, juce::Colour(0xfff4f5f7));
  setColour(juce::PopupMenu::textColourId, juce::Colour(0xff1f2226));
}

void LookAndFeel::drawRotarySlider(juce::Graphics &g, int x, int y, int width,
                                   int height, float sliderPos,
                                   float rotaryStartAngle, float rotaryEndAngle,
                                   juce::Slider &slider) {
  const auto bounds = juce::Rectangle<int>(x, y, width, height).toFloat();
  const float diameter = juce::jmin(bounds.getWidth(), bounds.getHeight(),
                                    float(Skin::maxKnobDiameter));
  const auto area =
      juce::Rectangle<float>(diameter, diameter).withCentre(bounds.getCentre());
  const auto centre = area.getCentre();

  const float outer = diameter * 0.5f;
  const float tickRadius = outer - juce::jmax(3.5f, outer * 0.1f);
  const float arcRadius = outer * 0.8f;
  const float skirtRadius = outer * 0.64f;
  const float capRadius = skirtRadius * 0.72f;
  const float angle =
      rotaryStartAngle + sliderPos * (rotaryEndAngle - rotaryStartAngle);

  // Knobs on the dark leather use light ink and a much dimmer lip.
  const auto ink = slider.findColour(juce::Slider::textBoxTextColourId);
  const float lip = ink.getPerceivedBrightness() > 0.5f ? 0.1f : 0.7f;

  // 1. Scale ticks engraved into the surface.
  for (int i = 0; i <= 10; ++i) {
    const float a =
        juce::jmap(float(i) / 10.0f, rotaryStartAngle, rotaryEndAngle);
    const auto from = centre.getPointOnCircumference(tickRadius, a);
    const auto to = centre.getPointOnCircumference(outer - 1.0f, a);

    g.setColour(juce::Colours::white.withAlpha(lip));
    g.drawLine(from.x, from.y + 1.0f, to.x, to.y + 1.0f, 1.2f);
    g.setColour(ink.withAlpha(0.8f));
    g.drawLine(from.x, from.y, to.x, to.y, 1.2f);
  }

  // 2. The recessed groove the value arc runs in.
  juce::Path track;
  track.addCentredArc(centre.x, centre.y, arcRadius, arcRadius, 0.0f,
                      rotaryStartAngle, rotaryEndAngle, true);
  const juce::PathStrokeType groove(4.5f, juce::PathStrokeType::curved,
                                    juce::PathStrokeType::rounded);
  g.setColour(juce::Colours::white.withAlpha(lip));
  g.strokePath(track, groove, juce::AffineTransform::translation(0.0f, 1.0f));
  g.setColour(juce::Colours::black.withAlpha(0.3f + (0.7f - lip) * 0.4f));
  g.strokePath(track, groove);

  // 3. The value, in UIKit blue. Bipolar parameters grow out from zero.
  const bool bipolar = slider.getMinimum() < 0.0 && slider.getMaximum() > 0.0;
  const float origin =
      bipolar ? juce::jmap(float(slider.valueToProportionOfLength(0.0)),
                           rotaryStartAngle, rotaryEndAngle)
              : rotaryStartAngle;

  if (std::abs(angle - origin) > 0.01f) {
    juce::Path value;
    value.addCentredArc(centre.x, centre.y, arcRadius, arcRadius, 0.0f,
                        juce::jmin(origin, angle), juce::jmax(origin, angle),
                        true);

    g.setColour(Skin::blue.withAlpha(0.22f));
    g.strokePath(value, juce::PathStrokeType(7.0f, juce::PathStrokeType::curved,
                                             juce::PathStrokeType::rounded));
    g.setGradientFill({Skin::blueLight, centre.x, centre.y - arcRadius,
                       Skin::blueDark, centre.x, centre.y + arcRadius, false});
    g.strokePath(value, juce::PathStrokeType(3.0f, juce::PathStrokeType::curved,
                                             juce::PathStrokeType::rounded));
  }

  // 4. The knob itself, from the cache.
  const float scale = g.getInternalContext().getPhysicalPixelScaleFactor();
  const auto key = std::make_pair(juce::roundToInt(diameter * 8.0f),
                                  juce::roundToInt(diameter * scale));
  if (knobCache.size() > 32)
    knobCache.clear(); // a live window resize walks through many sizes

  auto &body = knobCache[key];
  if (!body.isValid())
    body = Textures::knob(diameter, skirtRadius, capRadius, scale);
  g.setOpacity(1.0f); // drawImage inherits the last colour's alpha
  g.drawImage(body, area);

  const auto capArea =
      juce::Rectangle<float>(capRadius * 2.0f, capRadius * 2.0f)
          .withCentre(centre);
  if (slider.isMouseOverOrDragging()) {
    g.setColour(juce::Colours::white.withAlpha(0.08f));
    g.fillEllipse(capArea);
  }

  // 5. The pointer: a groove cut into the spun cap.
  const auto from = centre.getPointOnCircumference(capRadius * 0.25f, angle);
  const auto to = centre.getPointOnCircumference(capRadius * 0.84f, angle);
  g.setColour(juce::Colours::white.withAlpha(0.8f));
  g.drawLine(from.x, from.y + 0.8f, to.x, to.y + 0.8f, 2.0f);
  g.setColour(juce::Colour(0xff2b2f35));
  g.drawLine(from.x, from.y, to.x, to.y, 2.2f);
}

void LookAndFeel::drawLinearSlider(juce::Graphics &g, int x, int y, int width,
                                   int height, float sliderPos,
                                   float minSliderPos, float maxSliderPos,
                                   const juce::Slider::SliderStyle style,
                                   juce::Slider &slider) {
  if (slider.isBar() || !slider.isVertical()) {
    LookAndFeel_V4::drawLinearSlider(g, x, y, width, height, sliderPos,
                                     minSliderPos, maxSliderPos, style, slider);
    return;
  }

  // A UISlider stood on end: an inset pill, blue below the thumb and pale
  // grey above it, with the classic white glossy thumb. (minSliderPos and
  // maxSliderPos belong to two-value sliders; the travel comes from the
  // bounds, inset by the thumb radius just as Slider does.)
  const float trackWidth = 9.0f;
  const float radius = trackWidth * 0.5f;
  const float indent = float(getSliderThumbRadius(slider));
  const float centreX = float(x) + float(width) * 0.5f;
  const float top = float(y) + indent;
  const float bottom = float(y + height) - indent;
  const auto track = juce::Rectangle<float>(
      centreX - radius, top - radius, trackWidth, bottom - top + trackWidth);

  juce::Path trackPath;
  trackPath.addRoundedRectangle(track, radius);

  g.setColour(juce::Colours::white.withAlpha(0.75f));
  g.fillRoundedRectangle(track.translated(0.0f, 1.0f), radius);

  juce::ColourGradient empty(juce::Colour(0xffb3b5b8), track.getX(), 0.0f,
                             juce::Colour(0xffd8dadd), track.getRight(), 0.0f,
                             false);
  empty.addColour(0.35, juce::Colour(0xfff6f6f7));
  g.setGradientFill(empty);
  g.fillPath(trackPath);

  {
    juce::Graphics::ScopedSaveState state(g);
    juce::Path filled;
    filled.addRectangle(track.withTop(sliderPos));
    g.reduceClipRegion(filled);

    juce::ColourGradient blue(Skin::blueDark, track.getX(), 0.0f, Skin::blue,
                              track.getRight(), 0.0f, false);
    blue.addColour(0.35, Skin::blueLight);
    g.setGradientFill(blue);
    g.fillPath(trackPath);
  }

  Textures::drawInnerShadow(g, trackPath, juce::Colours::black.withAlpha(0.45f),
                            3, {0, 1});
  g.setColour(juce::Colours::black.withAlpha(0.4f));
  g.strokePath(trackPath, juce::PathStrokeType(1.0f));

  const auto thumb =
      juce::Rectangle<float>(faderThumbDiameter, faderThumbDiameter)
          .withCentre({centreX, sliderPos});
  juce::Path thumbPath;
  thumbPath.addEllipse(thumb);
  juce::DropShadow(juce::Colours::black.withAlpha(0.5f), 5, {0, 2})
      .drawForPath(g, thumbPath);

  const bool pressed = slider.isMouseButtonDown();
  juce::ColourGradient fill(
      pressed ? juce::Colour(0xffe2e2e2) : juce::Colours::white, centreX,
      thumb.getY(),
      pressed ? juce::Colour(0xffb4b4b4) : juce::Colour(0xffcfcfcf), centreX,
      thumb.getBottom(), false);
  fill.addColour(0.5,
                 pressed ? juce::Colour(0xffd2d2d2) : juce::Colour(0xffeeeeee));
  g.setGradientFill(fill);
  g.fillPath(thumbPath);

  g.setColour(juce::Colours::black.withAlpha(0.35f));
  g.drawEllipse(thumb.reduced(0.5f), 1.0f);
  g.setGradientFill({juce::Colours::white, centreX, thumb.getY(),
                     juce::Colours::transparentWhite, centreX,
                     thumb.getCentreY(), false});
  g.drawEllipse(thumb.reduced(1.5f), 1.0f);
}

int LookAndFeel::getSliderThumbRadius(juce::Slider &) {
  return juce::roundToInt(faderThumbDiameter * 0.5f);
}

juce::Slider::SliderLayout LookAndFeel::getSliderLayout(juce::Slider &slider) {
  juce::Slider::SliderLayout layout;
  auto bounds = slider.getLocalBounds();

  if (slider.getTextBoxPosition() == juce::Slider::TextBoxBelow) {
    const int boxHeight = slider.getTextBoxHeight();
    layout.textBoxBounds =
        bounds.removeFromBottom(boxHeight).withSizeKeepingCentre(
            juce::jmin(bounds.getWidth(), slider.getTextBoxWidth()), boxHeight);
  }

  // Faders keep their thumb (and its shadow) clear of the text at both ends.
  if (!slider.isRotary())
    bounds.reduce(0, 6);

  layout.sliderBounds = bounds;
  return layout;
}

juce::Label *LookAndFeel::createSliderTextBox(juce::Slider &slider) {
  auto *label = LookAndFeel_V4::createSliderTextBox(slider);
  label->setFont(Skin::font(12.0f));
  label->setColour(juce::Label::backgroundColourId,
                   juce::Colours::transparentBlack);
  label->setColour(juce::Label::outlineColourId,
                   juce::Colours::transparentBlack);
  return label;
}

void LookAndFeel::drawLabel(juce::Graphics &g, juce::Label &label) {
  // While editing, the label's TextEditor draws the field and the text.
  if (label.isBeingEdited())
    return;

  Textures::drawLetterpressText(
      g, label.getText(), label.getLocalBounds().toFloat(),
      label.getJustificationType(), label.findColour(juce::Label::textColourId),
      label.getFont());
}

void LookAndFeel::fillTextEditorBackground(juce::Graphics &g, int width,
                                           int height, juce::TextEditor &) {
  const auto area =
      juce::Rectangle<float>(float(width), float(height)).reduced(0.5f);
  juce::Path field;
  field.addRoundedRectangle(area, 4.0f);

  g.setColour(juce::Colours::white);
  g.fillPath(field);
  Textures::drawInnerShadow(g, field, juce::Colours::black.withAlpha(0.35f), 2,
                            {0, 1});
}

void LookAndFeel::drawTextEditorOutline(juce::Graphics &g, int width,
                                        int height, juce::TextEditor &editor) {
  const auto area =
      juce::Rectangle<float>(float(width), float(height)).reduced(0.5f);
  g.setColour(editor.hasKeyboardFocus(true)
                  ? Skin::blue
                  : juce::Colours::black.withAlpha(0.3f));
  g.drawRoundedRectangle(area, 4.0f, 1.2f);
}

void LookAndFeel::drawGroupComponentOutline(juce::Graphics &g, int w, int h,
                                            const juce::String &text,
                                            const juce::Justification &,
                                            juce::GroupComponent &) {
  auto bounds = juce::Rectangle<float>(float(w), float(h));
  const auto title = bounds.removeFromTop(float(Skin::groupTitleHeight));
  const auto well = bounds.reduced(0.5f).withTrimmedBottom(1.0f);
  const float radius = Skin::groupCornerRadius;

  juce::Path wellPath;
  wellPath.addRoundedRectangle(well, radius);

  // The lower lip of the cut catches the light from above.
  {
    juce::Graphics::ScopedSaveState state(g);
    g.reduceClipRegion(
        juce::Rectangle<int>(0, juce::roundToInt(well.getCentreY()), w, h));
    juce::Path lip;
    lip.addRoundedRectangle(well.translated(0.0f, 1.0f), radius);
    lip.addRoundedRectangle(well, radius);
    lip.setUsingNonZeroWinding(false);
    g.setColour(juce::Colours::white.withAlpha(0.7f));
    g.fillPath(lip);
  }

  g.setGradientFill({juce::Colours::black.withAlpha(0.1f), 0.0f, well.getY(),
                     juce::Colours::black.withAlpha(0.03f), 0.0f,
                     well.getBottom(), false});
  g.fillPath(wellPath);
  Textures::drawInnerShadow(g, wellPath, juce::Colours::black.withAlpha(0.4f),
                            6, {0, 2});
  g.setColour(juce::Colours::black.withAlpha(0.22f));
  g.strokePath(wellPath, juce::PathStrokeType(1.0f));

  // Section titles read like iOS grouped-table headers.
  Textures::drawLetterpressText(
      g, text.toUpperCase(),
      title.withTrimmedLeft(8.0f).withTrimmedBottom(5.0f),
      juce::Justification::bottomLeft, Skin::inkOnMetal,
      Skin::font(12.0f).withExtraKerningFactor(0.12f));
}

void LookAndFeel::drawPopupMenuBackground(juce::Graphics &g, int width,
                                          int height) {
  g.setGradientFill({juce::Colour(0xfffcfcfd), 0.0f, 0.0f,
                     juce::Colour(0xffe8eaee), 0.0f, float(height), false});
  g.fillAll();
  g.setColour(juce::Colours::black.withAlpha(0.3f));
  g.drawRect(0, 0, width, height);
}

void LookAndFeel::drawPopupMenuItem(
    juce::Graphics &g, const juce::Rectangle<int> &area, bool isSeparator,
    bool isActive, bool isHighlighted, bool isTicked, bool,
    const juce::String &text, const juce::String &, const juce::Drawable *,
    const juce::Colour *) {
  auto r = area.toFloat();

  if (isSeparator) {
    g.setColour(juce::Colours::black.withAlpha(0.15f));
    g.fillRect(r.withSizeKeepingCentre(r.getWidth() - 20.0f, 1.0f));
    return;
  }

  const bool selected = isHighlighted && isActive;
  if (selected) {
    // The iOS 6 table-cell selection blue.
    g.setGradientFill({juce::Colour(0xff058cf5), 0.0f, r.getY(),
                       juce::Colour(0xff015fe6), 0.0f, r.getBottom(), false});
    g.fillRect(r);
  } else {
    g.setColour(juce::Colours::black.withAlpha(0.08f));
    g.fillRect(r.withTop(r.getBottom() - 1.0f).reduced(8.0f, 0.0f));
  }

  auto ink = selected ? juce::Colours::white : juce::Colour(0xff1f2226);
  if (!isActive)
    ink = ink.withAlpha(0.4f);

  auto textArea = r.reduced(14.0f, 0.0f);
  const auto tickArea = textArea.removeFromRight(22.0f);
  Textures::drawLetterpressText(g, text, textArea,
                                juce::Justification::centredLeft, ink,
                                getPopupMenuFont());

  if (isTicked) {
    const auto c = tickArea.getCentre();
    juce::Path check;
    check.startNewSubPath(c.x - 6.0f, c.y);
    check.lineTo(c.x - 2.0f, c.y + 4.5f);
    check.lineTo(c.x + 6.0f, c.y - 5.0f);
    g.setColour(selected ? juce::Colours::white : juce::Colour(0xff385487));
    g.strokePath(check, juce::PathStrokeType(2.4f, juce::PathStrokeType::curved,
                                             juce::PathStrokeType::rounded));
  }
}

juce::Font LookAndFeel::getPopupMenuFont() { return Skin::font(15.0f); }

void LookAndFeel::getIdealPopupMenuItemSize(const juce::String &text,
                                            bool isSeparator, int,
                                            int &idealWidth, int &idealHeight) {
  if (isSeparator) {
    idealWidth = 50;
    idealHeight = 9;
    return;
  }

  idealWidth = juce::roundToInt(juce::GlyphArrangement::getStringWidth(
                   getPopupMenuFont(), text)) +
               60;
  idealHeight = 30;
}
