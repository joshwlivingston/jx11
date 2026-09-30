#include "Slider.h"
#include "Skin.h"
#include "Textures.h"
#include "juce_graphics/juce_graphics.h"
#include "juce_gui_basics/juce_gui_basics.h"

JX11Slider::JX11Slider() {}

JX11Slider::~JX11Slider() {}

void JX11Slider::resized() {
  auto bounds = getLocalBounds();

  // Knobs stop growing at a fixed size; keep the name, knob and value
  // together as one block, centred in whatever room is left.
  if (slider.isRotary()) {
    const int knobSize =
        juce::jmin(bounds.getWidth(), Skin::maxKnobDiameter,
                   bounds.getHeight() - labelHeight - textBoxHeight);
    bounds = bounds.withSizeKeepingCentre(
        bounds.getWidth(), labelHeight + knobSize + textBoxHeight);
  }

  labelArea = bounds.removeFromTop(labelHeight);
  slider.setBounds(bounds);
}

void JX11Slider::paint(juce::Graphics &g) {
  // Transparent: the control sits directly on whatever surface is behind it.
  Textures::drawLetterpressText(
      g, label.toUpperCase(), labelArea.toFloat(), juce::Justification::centred,
      slider.findColour(juce::Slider::textBoxTextColourId),
      Skin::font(11.0f).withExtraKerningFactor(0.08f));
}

void JX11Slider::setDefaultValue(double value) {
  slider.setDoubleClickReturnValue(true, value);
}

void JX11Slider::setInkColour(juce::Colour ink) {
  slider.setColour(juce::Slider::textBoxTextColourId, ink);
  repaint();
}
