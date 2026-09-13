#pragma once

#include <juce_gui_basics/juce_gui_basics.h>

#include "RotaryKnob.h"

class OscillatorGroup : public juce::Component {
public:
  OscillatorGroup(juce::String groupLabel, RotaryKnob &mix, RotaryKnob &tune,
                  RotaryKnob &fine);
  ~OscillatorGroup() override;

  void resized() override;

  juce::String label;

private:
  juce::GroupComponent group;
  RotaryKnob &mixKnob, &tuneKnob, &fineKnob;

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(OscillatorGroup)
};
