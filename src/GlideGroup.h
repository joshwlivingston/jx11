#pragma once

#include <juce_gui_basics/juce_gui_basics.h>

#include "RotaryKnob.h"
#include "SegmentedControl.h"

class GlideGroup : public juce::Component {
public:
  GlideGroup(juce::String groupLabel, SegmentedControl &glideMode,
             RotaryKnob &glideRate, RotaryKnob &glideBend);
  ~GlideGroup() override;

  void resized() override;

  juce::String label;

private:
  juce::GroupComponent group;
  SegmentedControl &glideModeSelector;
  RotaryKnob &glideRateKnob, &glideBendKnob;

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(GlideGroup)
};
