#pragma once

#include <juce_gui_basics/juce_gui_basics.h>

#include "RotaryKnob.h"

class VibratoGroup : public juce::Component {
public:
  VibratoGroup(juce::String groupLabel, RotaryKnob &rate, RotaryKnob &vibrato);
  ~VibratoGroup() override;

  void resized() override;

  juce::String label;

private:
  juce::GroupComponent group;
  RotaryKnob &lfoRateKnob, &vibratoKnob;

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(VibratoGroup)
};
