#include "RotaryKnob.h"
#include "juce_core/juce_core.h"
#include "juce_gui_basics/juce_gui_basics.h"

RotaryKnob::RotaryKnob() {
  slider.setSliderStyle(
      juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
  slider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 80, textBoxHeight);
  slider.setRotaryParameters(juce::degreesToRadians(225.0f),
                             juce::degreesToRadians(495.0f), true);
  addAndMakeVisible(slider);
}
