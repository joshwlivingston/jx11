#include "LookAndFeel.h"
#include <cmath>

LookAndFeel::LookAndFeel() {
  setColour(juce::ResizableWindow::backgroundColourId, chassisColor);
  setColour(juce::Label::textColourId, silkScreenWhite);
  setColour(juce::Slider::textBoxTextColourId, silkScreenWhite);

  // Deeply recessed text boxes
  setColour(juce::Slider::textBoxBackgroundColourId, juce::Colour(5, 6, 7));
  setColour(juce::Slider::textBoxOutlineColourId,
            juce::Colours::black.withAlpha(0.8f));
  setColour(juce::Slider::trackColourId, amberGlow);
}

// Helper: True Soft Drop Shadows
void LookAndFeel::drawSoftShadow(juce::Graphics &g,
                                 juce::Rectangle<float> bounds, float offset,
                                 float radius, float opacity) {
  juce::Colour shadowColor = juce::Colours::black.withAlpha(opacity);
  juce::Colour transparent = juce::Colours::black.withAlpha(0.0f);

  juce::Rectangle<float> shadowBounds =
      bounds.translated(offset, offset * 1.5f).expanded(radius);
  juce::ColourGradient shadowGrad(
      shadowColor, shadowBounds.getCentreX(), shadowBounds.getCentreY(),
      transparent, shadowBounds.getRight(), shadowBounds.getCentreY(), true);
  g.setGradientFill(shadowGrad);
  g.fillEllipse(shadowBounds);
}

void LookAndFeel::drawRotarySlider(juce::Graphics &g, int x, int y, int width,
                                   int height, float sliderPos,
                                   float rotaryStartAngle, float rotaryEndAngle,
                                   juce::Slider &slider) {
  auto bounds = juce::Rectangle<int>(x, y, width, height).toFloat();
  auto center = bounds.getCentre();
  auto radius = juce::jmin(bounds.getWidth(), bounds.getHeight()) / 2.0f - 8.0f;
  auto toAngle =
      rotaryStartAngle + sliderPos * (rotaryEndAngle - rotaryStartAngle);

  // 1. THE SILKSCREENED TICK RING (Now with subtle embossing)
  float ringRadius = radius + 6.0f;
  for (int i = 0; i <= 10; ++i) {
    float angle =
        rotaryStartAngle + (i / 10.0f) * (rotaryEndAngle - rotaryStartAngle);
    float cosA = std::cos(angle - juce::MathConstants<float>::halfPi);
    float sinA = std::sin(angle - juce::MathConstants<float>::halfPi);

    juce::Point<float> p1(center.x + ringRadius * cosA,
                          center.y + ringRadius * sinA);
    juce::Point<float> p2(center.x + (ringRadius + 4.0f) * cosA,
                          center.y + (ringRadius + 4.0f) * sinA);

    // Ink thickness shadow
    g.setColour(juce::Colours::black.withAlpha(0.3f));
    g.drawLine(p1.x, p1.y + 1.0f, p2.x, p2.y + 1.0f, 1.5f);
    // Silkscreen ink
    g.setColour(silkScreenWhite);
    g.drawLine(p1.x, p1.y, p2.x, p2.y, 1.5f);
  }

  // 2. THE VOLUMETRIC SOFT SHADOW
  juce::Rectangle<float> knobBounds(center.x - radius, center.y - radius,
                                    radius * 2.0f, radius * 2.0f);
  drawSoftShadow(g, knobBounds, 3.0f, 4.0f, 0.6f);

  // Contact shadow (sharp, ambient occlusion)
  g.setColour(juce::Colours::black.withAlpha(0.8f));
  g.fillEllipse(knobBounds.translated(0.0f, 1.0f).reduced(1.0f));

  // 3. BAKELITE SKIRT (The knurled fluting)
  // We draw subtle ridges around the edge before the main dome
  for (float a = 0; a < juce::MathConstants<float>::twoPi; a += 0.2f) {
    float cx = center.x + (radius - 1.0f) * std::cos(a);
    float cy = center.y + (radius - 1.0f) * std::sin(a);
    g.setColour(knobBaseColor.darker(0.8f));
    g.fillEllipse(cx - 2.0f, cy - 2.0f, 4.0f, 4.0f);
  }

  // 4. THE MAIN KNOB DOME (Subsurface depth)
  juce::ColourGradient baseGrad(
      knobBaseColor.brighter(0.1f), center.x, center.y - radius * 0.5f,
      knobBaseColor.darker(0.6f), center.x, center.y + radius, false);
  g.setGradientFill(baseGrad);
  g.fillEllipse(knobBounds);

  // 5. THE SPHERICAL HIGHLIGHT (Light wraps around the plastic)
  juce::ColourGradient specularGrad(
      juce::Colours::white.withAlpha(0.15f), center.x - radius * 0.3f,
      center.y - radius * 0.3f, juce::Colours::transparentWhite,
      center.x + radius * 0.3f, center.y + radius * 0.3f, true);
  g.setGradientFill(specularGrad);
  g.fillEllipse(knobBounds.reduced(2.0f));

  // 6. TOP CAP AND BEVEL
  float capRadius = radius * 0.70f;
  juce::Rectangle<float> capBounds(center.x - capRadius, center.y - capRadius,
                                   capRadius * 2.0f, capRadius * 2.0f);

  // Bevel rim highlight catching the top-left light
  g.setColour(juce::Colours::white.withAlpha(0.1f));
  g.drawEllipse(capBounds, 1.5f);
  // Bevel rim shadow on the bottom right
  g.setColour(juce::Colours::black.withAlpha(0.6f));
  g.drawEllipse(capBounds.translated(0.5f, 0.5f), 1.5f);

  juce::ColourGradient capGrad(knobBaseColor, capBounds.getX(),
                               capBounds.getY(), knobBaseColor.darker(0.3f),
                               capBounds.getRight(), capBounds.getBottom(),
                               false);
  g.setGradientFill(capGrad);
  g.fillEllipse(capBounds);

  // 7. PHYSICAL INDICATOR (A routed channel filled with bright enamel)
  float arg = toAngle - juce::MathConstants<float>::halfPi;
  float indLength = capRadius - 2.0f;

  juce::Path indicator;
  indicator.startNewSubPath(center);
  indicator.lineTo(center.x + indLength * std::cos(arg),
                   center.y + indLength * std::sin(arg));

  // Routed shadow (Ambient occlusion inside the rut)
  g.setColour(juce::Colours::black.withAlpha(0.9f));
  g.strokePath(indicator,
               juce::PathStrokeType(3.0f, juce::PathStrokeType::mitered,
                                    juce::PathStrokeType::rounded));

  // The wet enamel paint sitting inside the rut
  g.setColour(silkScreenWhite);
  g.strokePath(indicator,
               juce::PathStrokeType(2.0f, juce::PathStrokeType::mitered,
                                    juce::PathStrokeType::rounded));
}

void LookAndFeel::drawLinearSlider(juce::Graphics &g, int x, int y, int width,
                                   int height, float sliderPos,
                                   float minSliderPos, float maxSliderPos,
                                   const juce::Slider::SliderStyle style,
                                   juce::Slider &slider) {
  if (slider.isBar())
    return;

  bool isHorizontal = slider.isHorizontal();
  auto bounds =
      juce::Rectangle<float>((float)x, (float)y, (float)width, (float)height);

  juce::Point<float> start(
      isHorizontal ? bounds.getX() + 20 : bounds.getCentreX(),
      isHorizontal ? bounds.getCentreY() : bounds.getBottom() - 20);
  juce::Point<float> end(
      isHorizontal ? bounds.getRight() - 20 : bounds.getCentreX(),
      isHorizontal ? bounds.getCentreY() : bounds.getY() + 20);

  // 1. THE ROUTED CHASSIS SLOT
  juce::Path trackPath;
  trackPath.startNewSubPath(start);
  trackPath.lineTo(end);

  // Milled aluminum lip highlight (bottom/right edge)
  g.setColour(juce::Colours::white.withAlpha(0.12f));
  g.strokePath(trackPath,
               {(float)faderTrackWidth + 1.0f, juce::PathStrokeType::mitered,
                juce::PathStrokeType::rounded},
               juce::AffineTransform::translation(0.5f, 1.0f));

  // Deep cast shadow inside the slot
  g.setColour(juce::Colours::black.withAlpha(0.9f));
  g.strokePath(trackPath,
               {(float)faderTrackWidth, juce::PathStrokeType::mitered,
                juce::PathStrokeType::rounded});

  // 2. THE RUBBER DUST GUARD
  // A textured, matte rubber flap that blocks light
  g.setColour(juce::Colour(12, 13, 14));
  g.strokePath(trackPath,
               {(float)faderTrackWidth - 2.0f, juce::PathStrokeType::mitered,
                juce::PathStrokeType::rounded});

  // The slit where the two rubber flaps meet
  g.setColour(juce::Colours::black);
  g.strokePath(trackPath, {1.0f, juce::PathStrokeType::mitered,
                           juce::PathStrokeType::butt});

  // 3. THE MACHINED FADER CAP
  float capW = isHorizontal ? 26.0f : 36.0f;
  float capH = isHorizontal ? 36.0f : 26.0f;
  juce::Point<float> thumbPos(isHorizontal ? sliderPos : bounds.getCentreX(),
                              isHorizontal ? bounds.getCentreY() : sliderPos);
  juce::Rectangle<float> capBounds(capW, capH);
  capBounds.setCentre(thumbPos);

  // Majestic, elevated soft shadow. Faders sit high.
  drawSoftShadow(g, capBounds, 4.0f, 6.0f, 0.5f);
  // Contact shadow
  g.setColour(juce::Colours::black.withAlpha(0.6f));
  g.fillRoundedRectangle(capBounds.translated(0.0f, 2.0f), 3.0f);

  // ENVIRONMENTAL REFLECTION GRADIENT (The secret to skeuomorphic metal)
  juce::Colour baseMetal = juce::Colour(210, 215, 220);
  juce::Colour horizon = juce::Colour(150, 155, 160);

  juce::ColourGradient metalGrad;
  if (isHorizontal) {
    metalGrad = juce::ColourGradient(
        baseMetal, capBounds.getX(), capBounds.getY(), baseMetal.darker(0.2f),
        capBounds.getX(), capBounds.getBottom(), false);
    metalGrad.addColour(0.4f, baseMetal.brighter(0.2f)); // Upward reflection
    metalGrad.addColour(0.5f, horizon);                  // Horizon line block
    metalGrad.addColour(0.6f, horizon.brighter(0.1f));
  } else {
    metalGrad = juce::ColourGradient(
        baseMetal, capBounds.getX(), capBounds.getY(), baseMetal.darker(0.2f),
        capBounds.getRight(), capBounds.getY(), false);
    metalGrad.addColour(0.4f, baseMetal.brighter(0.2f));
    metalGrad.addColour(0.5f, horizon);
    metalGrad.addColour(0.6f, horizon.brighter(0.1f));
  }
  g.setGradientFill(metalGrad);
  g.fillRoundedRectangle(capBounds, 3.0f);

  // Milled top-edge highlight
  g.setColour(juce::Colours::white.withAlpha(0.7f));
  g.drawRoundedRectangle(capBounds.reduced(0.5f), 3.0f, 1.0f);

  // 4. THE TACTILE DIVOT
  drawFaderDivot(g, capBounds, isHorizontal);
}

void LookAndFeel::drawFaderDivot(juce::Graphics &g,
                                 juce::Rectangle<float> bounds,
                                 bool isHorizontal) {
  juce::Rectangle<float> divot =
      bounds.reduced(isHorizontal ? 5.0f : 10.0f, isHorizontal ? 10.0f : 5.0f);

  // Concave lighting: the shadow is caught on the top-left inner rim, light
  // catches the bottom-right inner rim.
  juce::ColourGradient divotGrad(juce::Colours::black.withAlpha(0.6f),
                                 divot.getX(), divot.getY(),
                                 juce::Colours::white.withAlpha(0.4f),
                                 divot.getRight(), divot.getBottom(), false);
  g.setGradientFill(divotGrad);
  g.fillRoundedRectangle(divot, 2.0f);

  // Inner rim shadow to give the carve absolute depth
  g.setColour(juce::Colours::black.withAlpha(0.3f));
  g.drawRoundedRectangle(divot, 2.0f, 1.0f);

  // The painted reference line (thick, slightly wet looking ink)
  g.setColour(juce::Colour(15, 15, 15));
  if (isHorizontal) {
    g.fillRect(divot.getCentreX() - 1.5f, divot.getY() + 2.0f, 3.0f,
               divot.getHeight() - 4.0f);
  } else {
    g.fillRect(divot.getX() + 2.0f, divot.getCentreY() - 1.5f,
               divot.getWidth() - 4.0f, 3.0f);
  }
}

void LookAndFeel::drawGroupComponentOutline(juce::Graphics &g, int w, int h,
                                            const juce::String &text,
                                            const juce::Justification &position,
                                            juce::GroupComponent &group) {
  auto panelBounds =
      juce::Rectangle<float>(0.0f, 8.0f, (float)w, (float)h - 8.0f)
          .reduced(4.0f);

  // 1. BOLTED SUB-PANEL WITH PROPER DROP SHADOW
  drawSoftShadow(g, panelBounds, 2.0f, 6.0f, 0.4f);

  // The chassis material
  juce::Colour panelColor = chassisColor.brighter(0.08f);
  g.setColour(panelColor);
  g.fillRoundedRectangle(panelBounds, 6.0f);

  // True milled edges (1px light top/left, 1px dark bottom/right)
  g.setColour(juce::Colours::white.withAlpha(0.12f));
  g.drawLine(panelBounds.getX() + 6.0f, panelBounds.getY(),
             panelBounds.getRight() - 6.0f, panelBounds.getY(), 1.0f); // Top
  g.drawLine(panelBounds.getX(), panelBounds.getY() + 6.0f, panelBounds.getX(),
             panelBounds.getBottom() - 6.0f, 1.0f); // Left

  g.setColour(juce::Colours::black.withAlpha(0.5f));
  g.drawLine(panelBounds.getX() + 6.0f, panelBounds.getBottom(),
             panelBounds.getRight() - 6.0f, panelBounds.getBottom(),
             1.0f); // Bottom
  g.drawLine(panelBounds.getRight(), panelBounds.getY() + 6.0f,
             panelBounds.getRight(), panelBounds.getBottom() - 6.0f,
             1.0f); // Right

  // 2. HARDWARE SCREWS (Slightly randomized rotation for authenticity)
  float inset = 14.0f;
  drawScrewHead(g, panelBounds.getX() + inset, panelBounds.getY() + inset,
                0.45f);
  drawScrewHead(g, panelBounds.getRight() - inset, panelBounds.getY() + inset,
                1.12f);
  drawScrewHead(g, panelBounds.getX() + inset, panelBounds.getBottom() - inset,
                -0.65f);
  drawScrewHead(g, panelBounds.getRight() - inset,
                panelBounds.getBottom() - inset, 0.88f);

  // 3. SILKSCREENED HEADER TEXT WITH INK RELIEF
  if (text.isNotEmpty()) {
    g.setFont(getGroupComponentFont());
    juce::Rectangle<float> textBounds(panelBounds.getX() + 24.0f,
                                      panelBounds.getY() + 12.0f,
                                      panelBounds.getWidth() - 48.0f, 20.0f);

    // Subtly raised ink shadow (opposite of an inset shadow)
    g.setColour(juce::Colours::black.withAlpha(0.5f));
    g.drawText(text, textBounds.translated(0.0f, 1.0f),
               juce::Justification::topLeft, false);

    g.setColour(silkScreenWhite);
    g.drawText(text, textBounds, juce::Justification::topLeft, false);

    // Divider Line (grooved into the metal)
    float lineY = textBounds.getBottom() + 2.0f;
    g.setColour(juce::Colours::black.withAlpha(0.4f));
    g.drawLine(textBounds.getX(), lineY, textBounds.getX() + 120.0f, lineY,
               1.0f);
    g.setColour(juce::Colours::white.withAlpha(0.1f));
    g.drawLine(textBounds.getX(), lineY + 1.0f, textBounds.getX() + 120.0f,
               lineY + 1.0f, 1.0f);
  }
}

void LookAndFeel::drawScrewHead(juce::Graphics &g, float x, float y,
                                float rotation) {
  float radius = 3.8f;
  juce::Rectangle<float> bounds(x - radius, y - radius, radius * 2.0f,
                                radius * 2.0f);

  // Drilled hole ambient occlusion (recessed into the panel)
  g.setColour(juce::Colours::black.withAlpha(0.8f));
  g.fillEllipse(bounds.expanded(1.0f).translated(0.0f, 1.0f));

  // Stamped steel screw head (Metallic conical simulation)
  juce::Colour base = juce::Colour(180, 185, 190);
  juce::ColourGradient screwGrad(base.brighter(0.2f), bounds.getX(),
                                 bounds.getY(), base.darker(0.5f),
                                 bounds.getRight(), bounds.getBottom(), false);
  g.setGradientFill(screwGrad);
  g.fillEllipse(bounds);

  // The Phillips cross slot
  juce::Path cross;
  float slotRadius = radius * 0.65f;
  cross.addLineSegment(juce::Line<float>(x - slotRadius, y, x + slotRadius, y),
                       1.4f);
  cross.addLineSegment(juce::Line<float>(x, y - slotRadius, x, y + slotRadius),
                       1.4f);
  cross.applyTransform(juce::AffineTransform::rotation(rotation, x, y));

  // To make the slot look punched, it needs a shadow on the leading edge and a
  // highlight on the trailing edge. Instead of drawing a flat stroke, we stroke
  // it twice, offset.
  g.setColour(juce::Colours::white.withAlpha(0.4f));
  g.strokePath(cross, juce::PathStrokeType(1.0f),
               juce::AffineTransform::translation(0.5f, 0.5f));

  g.setColour(juce::Colours::black.withAlpha(0.9f));
  g.strokePath(cross, juce::PathStrokeType(1.4f));
}

int LookAndFeel::getSliderThumbRadius(juce::Slider &slider) { return 18; }

juce::Font LookAndFeel::getGroupComponentFont() {
  // Tracking/kerning matters in hardware!
  return juce::Font{juce::FontOptions{13.0f, juce::Font::bold}}
      .withExtraKerningFactor(0.05f);
}
