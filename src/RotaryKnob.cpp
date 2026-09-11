#include "RotaryKnob.h"
#include "juce_graphics/juce_graphics.h"
#include "juce_gui_basics/juce_gui_basics.h"

static constexpr int labelHeight = 15;
static constexpr int textBoxHeight = 20;

RotaryKnob::RotaryKnob() {
  slider.setSliderStyle(
      juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
  slider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 100, textBoxHeight);
  addAndMakeVisible(slider);

  setBounds(0, 0, 100, 120);
}

RotaryKnob::~RotaryKnob() {}

void RotaryKnob::resized() {
  auto bounds = getLocalBounds();
  slider.setBounds(0, labelHeight, bounds.getWidth(),
                   bounds.getHeight() - labelHeight);
}

void RotaryKnob::paint(juce::Graphics &g) {
  g.fillAll(
      getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));
  g.setFont(15.0f);
  g.setColour(juce::Colours::white);

  auto bounds = getLocalBounds();
  g.drawText(label, juce::Rectangle<int>{0, 0, bounds.getWidth(), labelHeight},
             juce::Justification::centred);
}
