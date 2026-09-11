#pragma once

#include "juce_core/juce_core.h"
#include "juce_core/system/juce_PlatformDefs.h"
#include "juce_gui_basics/juce_gui_basics.h"

class RotaryKnob : public juce::Component {
public:
  RotaryKnob();
  ~RotaryKnob() override;

  void paint(juce::Graphics &) override;
  void resized() override;

  juce::Slider slider;
  juce::String label;

private:
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(RotaryKnob)
};
