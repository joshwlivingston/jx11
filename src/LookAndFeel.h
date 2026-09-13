#pragma once

#include "juce_graphics/juce_graphics.h"
#include <juce_gui_basics/juce_gui_basics.h>

class LookAndFeel : public juce::LookAndFeel_V4 {
public:
  LookAndFeel();
  void drawRotarySlider(juce::Graphics &g, int x, int y, int width, int height,
                        float sliderPos, float rotaryStartAngle,
                        float rotaryEndAngle, juce::Slider &slider) override;

  void drawLinearSlider(juce::Graphics &g, int x, int y, int width, int height,
                        float sliderPos, float minSliderPos, float maxSliderPos,
                        const juce::Slider::SliderStyle style,
                        juce::Slider &slider) override;
  int getSliderThumbRadius(juce::Slider &slider) override;

private:
  // Set background color
  const juce::Colour backgroundColor = juce::Colour(30, 73, 90);

  // Set size of track and pointer for VerticalSlider
  static constexpr const int sliderTrackWidth = 24;

  // Helpers to adjust colors
  static const juce::Colour adjustLightness(const juce::Colour &color,
                                            float factor);
  static const juce::Colour gray(const juce::Colour &color, float factor);
  static const juce::Colour makeOutline(const juce::Colour &color,
                                        float factor);

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(LookAndFeel);
};
