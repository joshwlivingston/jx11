#pragma once

#include <juce_graphics/juce_graphics.h>
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

  void drawGroupComponentOutline(juce::Graphics &g, int w, int h,
                                 const juce::String &text,
                                 const juce::Justification &position,
                                 juce::GroupComponent &group) override;

  int getSliderThumbRadius(juce::Slider &slider) override;
  juce::Font getGroupComponentFont();

private:
  // The Palette: True Studio Hardware
  const juce::Colour chassisColor =
      juce::Colour(42, 46, 50); // Bead-blasted aluminum
  const juce::Colour knobBaseColor =
      juce::Colour(12, 12, 14); // Deep, dense Bakelite
  const juce::Colour amberGlow = juce::Colour(255, 140, 0);
  const juce::Colour silkScreenWhite =
      juce::Colour(235, 240, 245)
          .withAlpha(0.85f); // Ink isn't perfectly opaque

  static constexpr const int faderTrackWidth = 12;

  // The Global Light Source (Top Left, 135 degrees)
  const float lightAngle = -juce::MathConstants<float>::pi * 0.75f;

  void drawScrewHead(juce::Graphics &g, float x, float y, float rotation);
  void drawFaderDivot(juce::Graphics &g, juce::Rectangle<float> bounds,
                      bool isHorizontal);
  void drawSoftShadow(juce::Graphics &g, juce::Rectangle<float> bounds,
                      float offset, float radius, float opacity);

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(LookAndFeel);
};
