/*
  ==============================================================================

    VerticalSlider.cpp
    Created: 12 Sep 2026 9:28:51am
    Author:  Josh Livingston

  ==============================================================================
*/

#include "VerticalSlider.h"
#include "juce_gui_basics/juce_gui_basics.h"

//==============================================================================

VerticalSlider::VerticalSlider() {
  slider.setSliderStyle(juce::Slider::SliderStyle::LinearVertical);
  slider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 100, textBoxHeight);
  addAndMakeVisible(slider);

  setBounds(0, 0, 75, 150);
}
