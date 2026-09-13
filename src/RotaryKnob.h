#pragma once

#include "Slider.h"

class RotaryKnob : public JX11Slider {
public:
  RotaryKnob();

private:
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(RotaryKnob)
};
