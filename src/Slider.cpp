#include "Slider.h"
#include "juce_graphics/juce_graphics.h"
#include "juce_gui_basics/juce_gui_basics.h"

JX11Slider::JX11Slider() {}

JX11Slider::~JX11Slider() {}

void JX11Slider::resized() {
  auto bounds = getLocalBounds();
  slider.setBounds(0, labelHeight, bounds.getWidth(),
                   bounds.getHeight() - labelHeight);
}

void JX11Slider::paint(juce::Graphics &g) {
  g.fillAll(
      getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));
  g.setFont(15.0f);
  g.setColour(getLookAndFeel().findColour(juce::Label::textColourId));
  auto bounds = getLocalBounds();
  g.drawText(label, juce::Rectangle<int>{0, 0, bounds.getWidth(), labelHeight},
             juce::Justification::centred);

  // rectangle outlines for debugging
  // g.setColour(juce::Colours::red);
  // g.drawRect(getLocalBounds(), 1);

  // g.setColour(juce::Colours::yellow);
  // g.drawRect(0, labelHeight, bounds.getWidth(),
  //            bounds.getHeight() - labelHeight - textBoxHeight, 1);

  // g.setColour(juce::Colours::green);
}
