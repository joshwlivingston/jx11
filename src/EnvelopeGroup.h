#pragma once

#include <juce_gui_basics/juce_gui_basics.h>

#include "Slider.h"

class EnvelopeGroup : public juce::Component {
public:
  EnvelopeGroup(juce::String groupLabel, JX11Slider &attack, JX11Slider &decay,
                JX11Slider &sustain, JX11Slider &release);
  ~EnvelopeGroup() override;

  void resized() override;

  juce::String label;

private:
  juce::GroupComponent group;
  JX11Slider &attackSlider, &decaySlider, &sustainSlider, &releaseSlider;

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(EnvelopeGroup)
};
