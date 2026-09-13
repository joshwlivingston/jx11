
#pragma once

#include <juce_gui_basics/juce_gui_basics.h>

static constexpr int labelHeight = 15;
static constexpr int textBoxHeight = 20;

class JX11Slider : public juce::Component {
public:
  JX11Slider();
  ~JX11Slider() override;

  void paint(juce::Graphics &) override;
  void resized() override;

  juce::Slider slider;
  juce::String label;

private:
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(JX11Slider)
};
