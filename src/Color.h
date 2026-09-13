#pragma once

#include <juce_graphics/juce_graphics.h>
class color : public juce::Colour {
public:
  juce::Colour withLightnessFactor2();

private:
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(color);
};
