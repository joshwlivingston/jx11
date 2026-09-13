#pragma once

#include <juce_gui_basics/juce_gui_basics.h>

#include "RotaryKnob.h"

class FilterKnobGroup : public juce::Component {
public:
  FilterKnobGroup(juce::String groupLabel, RotaryKnob &freq, RotaryKnob &velo,
                  RotaryKnob &reso, RotaryKnob &env, RotaryKnob &lfo);
  ~FilterKnobGroup() override;

  void resized() override;

  juce::String label;

private:
  juce::GroupComponent group;
  RotaryKnob &freqKnob, &veloKnob, &resoKnob, &envKnob, &lfoKnob;

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(FilterKnobGroup)
};
