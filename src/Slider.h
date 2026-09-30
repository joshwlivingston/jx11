#pragma once

#include <juce_gui_basics/juce_gui_basics.h>

static constexpr int labelHeight = 16;
static constexpr int textBoxHeight = 18;

class JX11Slider : public juce::Component {
public:
  JX11Slider();
  ~JX11Slider() override;

  void paint(juce::Graphics &) override;
  void resized() override;

  // Double-clicking the control returns it to this value.
  void setDefaultValue(double value);

  // Name and value ink; use a light ink on dark surfaces like the leather.
  void setInkColour(juce::Colour ink);

  juce::Slider slider;
  juce::String label;

private:
  juce::Rectangle<int> labelArea;

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(JX11Slider)
};
