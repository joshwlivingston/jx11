#include "LookAndFeel.h"
#include "juce_core/juce_core.h"
#include "juce_graphics/juce_graphics.h"
#include "juce_gui_basics/juce_gui_basics.h"
#include <cmath>

LookAndFeel::LookAndFeel() {
  setColour(juce::ResizableWindow::backgroundColourId,
            juce::Colour(30, 60, 90));

  setColour(juce::Slider::rotarySliderOutlineColourId, juce::Colour(0, 0, 0));
  setColour(juce::Slider::rotarySliderFillColourId, juce::Colour(90, 180, 240));
  setColour(juce::Slider::thumbColourId, juce::Colour(255, 255, 255));

  // Adjust colors on buttons to make on/off  (or different values) better
  // differentiated
  setColour(juce::TextButton::buttonColourId, juce::Colour(15, 30, 45));
  setColour(juce::TextButton::buttonOnColourId, juce::Colour(90, 180, 240));
  setColour(juce::TextButton::textColourOffId, juce::Colour(180, 180, 180));
  setColour(juce::TextButton::textColourOnId, juce::Colour(255, 255, 255));
  setColour(juce::ComboBox::outlineColourId, juce::Colour(180, 180, 180));
}

void LookAndFeel::drawRotarySlider(juce::Graphics &g, int x, int y, int width,
                                   int height, float sliderPos,
                                   float rotaryStartAngle, float rotaryEndAngle,
                                   juce::Slider &slider) {
  auto outlineColor = slider.findColour(juce::Slider::rotarySliderFillColourId);

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

    auto trackWidth =
        juce::jmin(6.0f, slider.isHorizontal() ? (float)height * 0.25f
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
