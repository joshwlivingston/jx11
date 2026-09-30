#include "PresetDisplay.h"
#include "Skin.h"
#include "Textures.h"

PresetDisplay::StepButton::StepButton(bool left)
    : juce::Button(left ? "Previous preset" : "Next preset"), pointsLeft(left) {
  setMouseCursor(juce::MouseCursor::PointingHandCursor);
}

void PresetDisplay::StepButton::paintButton(juce::Graphics &g, bool isOver,
                                            bool isDown) {
  const float size = float(juce::jmin(getWidth(), getHeight())) - 4.0f;
  const auto button =
      juce::Rectangle<float>(size, size)
          .withCentre(
              getLocalBounds().toFloat().getCentre().translated(0.0f, -1.0f));

  juce::Path shape;
  shape.addEllipse(button);
  juce::DropShadow(juce::Colours::black.withAlpha(0.7f), 3, {0, 1})
      .drawForPath(g, shape);

  // Polished chrome, with a horizon line across the middle.
  juce::ColourGradient chrome(juce::Colour(0xfff7f8f9), 0.0f, button.getY(),
                              juce::Colour(0xffb9bdc2), 0.0f,
                              button.getBottom(), false);
  chrome.addColour(0.49, juce::Colour(0xffdfe2e5));
  chrome.addColour(0.51, juce::Colour(0xffc2c6cb));
  g.setGradientFill(chrome);
  g.fillPath(shape);

  if (isDown) {
    Textures::drawInnerShadow(g, shape, juce::Colours::black.withAlpha(0.5f), 4,
                              {0, 2});
  } else if (isOver) {
    g.setColour(juce::Colours::white.withAlpha(0.15f));
    g.fillPath(shape);
  }

  g.setColour(juce::Colours::black.withAlpha(0.6f));
  g.drawEllipse(button.reduced(0.5f), 1.0f);

  // Play-style triangle, printed on the chrome.
  const auto c = button.getCentre();
  const float s = size * 0.2f;
  juce::Path arrow;
  if (pointsLeft)
    arrow.addTriangle(c.x + s * 0.7f, c.y - s, c.x + s * 0.7f, c.y + s,
                      c.x - s * 0.9f, c.y);
  else
    arrow.addTriangle(c.x - s * 0.7f, c.y - s, c.x - s * 0.7f, c.y + s,
                      c.x + s * 0.9f, c.y);

  g.setColour(juce::Colours::white.withAlpha(0.8f));
  g.fillPath(arrow, juce::AffineTransform::translation(0.0f, 1.0f));
  g.setColour(juce::Colour(0xff3d4148));
  g.fillPath(arrow);
}

PresetDisplay::PresetDisplay(juce::AudioProcessor &p) : processor(p) {
  previousButton.onClick = [this] { step(-1); };
  nextButton.onClick = [this] { step(1); };
  addAndMakeVisible(previousButton);
  addAndMakeVisible(nextButton);

  setMouseCursor(juce::MouseCursor::PointingHandCursor);

  // Hosts and MIDI program changes can switch presets behind our back.
  startTimerHz(10);
}

PresetDisplay::~PresetDisplay() {}

void PresetDisplay::resized() {
  auto bounds = getLocalBounds();
  const int buttonSize = juce::jmin(34, bounds.getHeight());

  previousButton.setBounds(bounds.removeFromLeft(buttonSize)
                               .withSizeKeepingCentre(buttonSize, buttonSize));
  nextButton.setBounds(bounds.removeFromRight(buttonSize)
                           .withSizeKeepingCentre(buttonSize, buttonSize));
  screen = bounds.reduced(12, 4);
}

void PresetDisplay::paint(juce::Graphics &g) {
  const auto glass = screen.toFloat();
  const float radius = 6.0f;

  // The bezel is sunk into the leather: a dark rim and a faint lit lip.
  const auto bezel = glass.expanded(4.0f);
  g.setColour(juce::Colours::white.withAlpha(0.12f));
  g.fillRoundedRectangle(bezel.translated(0.0f, 1.0f), radius + 3.0f);
  g.setGradientFill({juce::Colour(0xff0d0d0e), 0.0f, bezel.getY(),
                     juce::Colour(0xff34363a), 0.0f, bezel.getBottom(), false});
  g.fillRoundedRectangle(bezel, radius + 3.0f);

  juce::Path face;
  face.addRoundedRectangle(glass, radius);

  {
    juce::Graphics::ScopedSaveState state(g);
    g.reduceClipRegion(face);

    g.setGradientFill({Skin::lcdTop, 0.0f, glass.getY(), Skin::lcdBottom, 0.0f,
                       glass.getBottom(), false});
    g.fillRect(glass);

    // Backlight hot spot, then faint scanlines.
    g.setGradientFill({juce::Colours::white.withAlpha(0.35f),
                       glass.getCentreX(), glass.getCentreY(),
                       juce::Colours::transparentWhite, glass.getX(),
                       glass.getCentreY(), true});
    g.fillRect(glass);
    g.setColour(juce::Colours::black.withAlpha(0.025f));
    for (float y = glass.getY(); y < glass.getBottom(); y += 2.0f)
      g.fillRect(glass.getX(), y, glass.getWidth(), 1.0f);

    const int current = processor.getCurrentProgram();
    const int total = processor.getNumPrograms();
    auto text = glass.reduced(14.0f, 6.0f);
    const auto caption = text.removeFromTop(text.getHeight() * 0.36f);

    Textures::drawLetterpressText(
        g, "PROGRAM", caption, juce::Justification::centredLeft,
        Skin::lcdInk.withAlpha(0.7f),
        Skin::font(10.0f).withExtraKerningFactor(0.15f));
    Textures::drawLetterpressText(g,
                                  juce::String(current + 1).paddedLeft('0', 2) +
                                      " / " + juce::String(total),
                                  caption, juce::Justification::centredRight,
                                  Skin::lcdInk.withAlpha(0.7f),
                                  Skin::font(10.0f));
    Textures::drawLetterpressText(g, processor.getProgramName(current), text,
                                  juce::Justification::centredLeft,
                                  Skin::lcdInk, Skin::font(19.0f));

    Textures::drawInnerShadow(g, face, juce::Colours::black.withAlpha(0.6f), 5,
                              {0, 2});

    // A curved sheet of glass over the display.
    const auto sheen = juce::Rectangle<float>(
        glass.getX() - glass.getWidth() * 0.3f,
        glass.getY() - glass.getHeight() * 1.05f, glass.getWidth() * 1.6f,
        glass.getHeight() * 1.55f);
    g.setGradientFill({juce::Colours::white.withAlpha(0.4f), 0.0f, glass.getY(),
                       juce::Colours::white.withAlpha(0.06f), 0.0f,
                       sheen.getBottom(), false});
    g.fillEllipse(sheen);
  }

  g.setColour(juce::Colours::black.withAlpha(0.8f));
  g.strokePath(face, juce::PathStrokeType(1.0f));
}

void PresetDisplay::mouseUp(const juce::MouseEvent &e) {
  if (screen.contains(e.getPosition()))
    showPresetMenu();
}

void PresetDisplay::timerCallback() {
  const int current = processor.getCurrentProgram();
  if (current != shownProgram) {
    shownProgram = current;
    repaint();
  }
}

void PresetDisplay::selectProgram(int index) {
  if (index < 0 || index >= processor.getNumPrograms())
    return;

  processor.setCurrentProgram(index);
  processor.updateHostDisplay(
      juce::AudioProcessor::ChangeDetails().withProgramChanged(true));
  timerCallback();
}

void PresetDisplay::step(int delta) {
  const int total = processor.getNumPrograms();
  if (total > 0)
    selectProgram((processor.getCurrentProgram() + delta + total) % total);
}

void PresetDisplay::showPresetMenu() {
  juce::PopupMenu menu;
  menu.setLookAndFeel(&getLookAndFeel());

  const int current = processor.getCurrentProgram();
  for (int i = 0; i < processor.getNumPrograms(); ++i)
    menu.addItem(i + 1, processor.getProgramName(i), true, i == current);

  menu.showMenuAsync(juce::PopupMenu::Options()
                         .withTargetComponent(this)
                         .withTargetScreenArea(localAreaToGlobal(screen))
                         .withMinimumWidth(screen.getWidth())
                         .withMaximumNumColumns(2),
                     [safeThis = juce::Component::SafePointer<PresetDisplay>(
                          this)](int result) {
                       if (safeThis != nullptr && result > 0)
                         safeThis->selectProgram(result - 1);
                     });
}
