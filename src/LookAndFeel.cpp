#include "LookAndFeel.h"
#include "juce_core/juce_core.h"
#include "juce_graphics/juce_graphics.h"
#include "juce_gui_basics/juce_gui_basics.h"
#include <cmath>

LookAndFeel::LookAndFeel() {
  // Set all colors derived from bacgkround color
  static const juce::Colour textColor = backgroundColor.getLightness() < 0.5
                                            ? juce::Colour(255, 255, 255)
                                            : juce::Colour(0, 0, 0);
  static const juce::Colour textColorInverse =
      backgroundColor.getLightness() > 0.5 ? juce::Colour(255, 255, 255)
                                           : juce::Colour(0, 0, 0);
  static const juce::Colour activeColor =
      adjustLightness(backgroundColor, 2.0f);
  static const juce::Colour trackBackground =
      gray(adjustLightness(backgroundColor, 2.0f), 3.0f);
  static const juce::Colour offColor = gray(backgroundColor, 3.0f);
  static const juce::Colour outlineColor = makeOutline(backgroundColor, 2.0f);

  // Set background color
  setColour(juce::ResizableWindow::backgroundColourId, backgroundColor);

  // Shared by RotaryKnob and Vertical Slider
  setColour(juce::Slider::thumbColourId, textColor);
  setColour(juce::Label::textColourId, textColor);
  setColour(juce::Slider::textBoxTextColourId, textColor);
  setColour(juce::TextEditor::textColourId, textColor);
  setColour(juce::Slider::textBoxOutlineColourId, outlineColor);

  // RotaryKnob colors
  setColour(juce::Slider::rotarySliderOutlineColourId, offColor);
  setColour(juce::Slider::rotarySliderFillColourId, activeColor);

  // VerticalSlider colors
  setColour(juce::Slider::trackColourId, activeColor);
  setColour(juce::Slider::backgroundColourId, offColor);

  // Adjust colors on buttons to make on/off  (or different values) better
  // differentiated
  setColour(juce::TextButton::buttonColourId, offColor);
  setColour(juce::TextButton::buttonOnColourId, activeColor);
  setColour(juce::TextButton::textColourOffId, textColor);
  setColour(juce::TextButton::textColourOnId, textColorInverse);
  setColour(juce::ComboBox::outlineColourId, outlineColor);

  // Group Component colors
  setColour(juce::GroupComponent::outlineColourId, outlineColor);
  setColour(juce::GroupComponent::textColourId, textColor);
}

void LookAndFeel::drawRotarySlider(juce::Graphics &g, int x, int y, int width,
                                   int height, float sliderPos,
                                   float rotaryStartAngle, float rotaryEndAngle,
                                   juce::Slider &slider) {
  auto outlineColor =
      slider.findColour(juce::Slider::rotarySliderOutlineColourId);

  auto fillColor = slider.findColour(juce::Slider::rotarySliderFillColourId);
  auto dialColor = slider.findColour(juce::Slider::thumbColourId);

  auto bounds = juce::Rectangle<int>(x, y, width, width)
                    .toFloat()
                    .withTrimmedLeft(16.0f)
                    .withTrimmedRight(16.0f)
                    .withTrimmedTop(0.0f)
                    .withTrimmedBottom(8.0f);

  auto radius = bounds.getWidth() / 2.0f;
  auto toAngle =
      rotaryStartAngle + sliderPos * (rotaryEndAngle - rotaryStartAngle);
  auto lineW = 6.0f;
  auto arcRadius = radius - lineW / 2.0f;

  auto arg = toAngle - juce::MathConstants<float>::halfPi;
  auto dialW = 3.0f;
  auto dialRadius = arcRadius - 6.0f;

  auto center = bounds.getCentre();
  auto strokeType = juce::PathStrokeType(lineW, juce::PathStrokeType::curved,
                                         juce::PathStrokeType::butt);

  juce::Path backgroundArc;
  backgroundArc.addCentredArc(center.x, center.y, arcRadius, arcRadius, 0.0f,
                              rotaryStartAngle, rotaryEndAngle, true);
  g.setColour(outlineColor);
  g.strokePath(backgroundArc, strokeType);

  if (slider.isEnabled()) {
    juce::Path valueArc;
    valueArc.addCentredArc(center.x, center.y, arcRadius, arcRadius, 0.0f,
                           rotaryStartAngle, toAngle, true);
    g.setColour(fillColor);
    g.strokePath(valueArc, strokeType);
  }

  juce::Point<float> thumbPoint(center.x + dialRadius * std::cos(arg),
                                center.y + dialRadius * std::sin(arg));
  g.setColour(dialColor);
  g.drawLine(center.x, center.y, thumbPoint.x, thumbPoint.y, dialW);
  g.fillEllipse(juce::Rectangle<float>(dialW, dialW).withCentre(thumbPoint));
  g.fillEllipse(juce::Rectangle<float>(dialW, dialW).withCentre(center));
}

void LookAndFeel::drawLinearSlider(juce::Graphics &g, int x, int y, int width,
                                   int height, float sliderPos,
                                   float minSliderPos, float maxSliderPos,
                                   const juce::Slider::SliderStyle style,
                                   juce::Slider &slider) {
  if (slider.isBar()) {
    g.setColour(slider.findColour(juce::Slider::trackColourId));
    g.fillRect(
        slider.isHorizontal()
            ? juce::Rectangle<float>(static_cast<float>(x), (float)y + 0.5f,
                                     sliderPos - (float)x, (float)height - 1.0f)
            : juce::Rectangle<float>((float)x + 0.5f, sliderPos,
                                     (float)width - 1.0f,
                                     (float)y + ((float)height - sliderPos)));

    drawLinearSliderOutline(g, x, y, width, height, style, slider);
  } else {
    auto isTwoVal = (style == juce::Slider::SliderStyle::TwoValueVertical ||
                     style == juce::Slider::SliderStyle::TwoValueHorizontal);
    auto isThreeVal =
        (style == juce::Slider::SliderStyle::ThreeValueVertical ||
         style == juce::Slider::SliderStyle::ThreeValueHorizontal);

    auto trackWidth = juce::jmin((float)sliderTrackWidth,
                                 slider.isHorizontal() ? (float)height * 0.25f
                                                       : (float)width * 0.25f);

    juce::Point<float> startPoint(
        slider.isHorizontal() ? (float)x : (float)x + (float)width * 0.5f,
        slider.isHorizontal() ? (float)y + (float)height * 0.5f
                              : (float)(height + y));

    juce::Point<float> endPoint(
        slider.isHorizontal() ? (float)(width + x) : startPoint.x,
        slider.isHorizontal() ? startPoint.y : (float)y);

    juce::Path backgroundTrack;
    backgroundTrack.startNewSubPath(startPoint);
    backgroundTrack.lineTo(endPoint);
    g.setColour(slider.findColour(juce::Slider::backgroundColourId));
    g.strokePath(backgroundTrack, {trackWidth, juce::PathStrokeType::curved,
                                   juce::PathStrokeType::rounded});

    juce::Path valueTrack;
    juce::Point<float> minPoint, maxPoint, thumbPoint;

    if (isTwoVal || isThreeVal) {
      minPoint = {slider.isHorizontal() ? minSliderPos : (float)width * 0.5f,
                  slider.isHorizontal() ? (float)height * 0.5f : minSliderPos};

      if (isThreeVal)
        thumbPoint = {slider.isHorizontal() ? sliderPos : (float)width * 0.5f,
                      slider.isHorizontal() ? (float)height * 0.5f : sliderPos};

      maxPoint = {slider.isHorizontal() ? maxSliderPos : (float)width * 0.5f,
                  slider.isHorizontal() ? (float)height * 0.5f : maxSliderPos};
    } else {
      auto kx =
          slider.isHorizontal() ? sliderPos : ((float)x + (float)width * 0.5f);
      auto ky =
          slider.isHorizontal() ? ((float)y + (float)height * 0.5f) : sliderPos;

      minPoint = startPoint;
      maxPoint = {kx, ky};
    }

    auto thumbWidth = getSliderThumbRadius(slider);

    valueTrack.startNewSubPath(minPoint);
    valueTrack.lineTo(isThreeVal ? thumbPoint : maxPoint);
    g.setColour(slider.findColour(juce::Slider::trackColourId));
    g.strokePath(valueTrack, {trackWidth, juce::PathStrokeType::curved,
                              juce::PathStrokeType::rounded});

    if (!isTwoVal) {
      g.setColour(slider.findColour(juce::Slider::thumbColourId));
      g.fillEllipse(juce::Rectangle<float>(static_cast<float>(thumbWidth),
                                           static_cast<float>(thumbWidth))
                        .withCentre(isThreeVal ? thumbPoint : maxPoint));
    }

    if (isTwoVal || isThreeVal) {
      auto sr = juce::jmin(
          trackWidth,
          (slider.isHorizontal() ? (float)height : (float)width) * 0.4f);
      auto pointerColour = slider.findColour(juce::Slider::thumbColourId);

      if (slider.isHorizontal()) {
        drawPointer(g, minSliderPos - sr,
                    juce::jmax(0.0f, (float)y + (float)height * 0.5f -
                                         trackWidth * 2.0f),
                    trackWidth * 2.0f, pointerColour, 2);

        drawPointer(g, maxSliderPos - trackWidth,
                    juce::jmin((float)(y + height) - trackWidth * 2.0f,
                               (float)y + (float)height * 0.5f),
                    trackWidth * 2.0f, pointerColour, 4);
      } else {
        drawPointer(g,
                    juce::jmax(0.0f, (float)x + (float)width * 0.5f -
                                         trackWidth * 2.0f),
                    minSliderPos - trackWidth, trackWidth * 2.0f, pointerColour,
                    1);

        drawPointer(g,
                    juce::jmin((float)(x + width) - trackWidth * 2.0f,
                               (float)x + (float)width * 0.5f),
                    maxSliderPos - sr, trackWidth * 2.0f, pointerColour, 3);
      }
    }

    if (slider.isBar())
      drawLinearSliderOutline(g, x, y, width, height, style, slider);
  }
}

int LookAndFeel::getSliderThumbRadius(juce::Slider &slider) {
  return juce::jmin(sliderTrackWidth,
                    slider.isHorizontal()
                        ? static_cast<int>((float)slider.getHeight() * 0.5f)
                        : static_cast<int>((float)slider.getWidth() * 0.5f));
}

const juce::Colour LookAndFeel::adjustLightness(const juce::Colour &color,
                                                float factor) {
  if (color.getLightness() > 0.5f) {
    return color.withLightness(color.getLightness() / factor);
  }

  if (color.getLightness() < 0.5f) {
    float newLightness = color.getLightness() * factor;
    if (newLightness > 1.0f) {
      newLightness = 1.0f;
    }
    return color.withLightness(newLightness);
  }

  return color;
}

const juce::Colour LookAndFeel::gray(const juce::Colour &color, float factor) {
  return color.withSaturationHSL(color.getSaturationHSL() / factor);
}

const juce::Colour LookAndFeel::makeOutline(const juce::Colour &color,
                                            float factor) {
  return gray(adjustLightness(color, factor), factor);
}
