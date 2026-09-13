#include "EnvelopeGroup.h"
#include "juce_graphics/juce_graphics.h"

EnvelopeGroup::EnvelopeGroup(juce::String groupLabel, JX11Slider &attack,
                             JX11Slider &decay, JX11Slider &sustain,
                             JX11Slider &release)
    : label(groupLabel), attackSlider(attack), decaySlider(decay),
      sustainSlider(sustain), releaseSlider(release) {
  group.setText(label);
  group.setTextLabelPosition(juce::Justification::top);
  addAndMakeVisible(group);

  attackSlider.label = "Attack";
  addAndMakeVisible(attackSlider);

  decaySlider.label = "Decay";
  addAndMakeVisible(decaySlider);

  sustainSlider.label = "Sustain";
  addAndMakeVisible(sustainSlider);

  releaseSlider.label = "Release";
  addAndMakeVisible(releaseSlider);
}

EnvelopeGroup::~EnvelopeGroup() {}

void EnvelopeGroup::resized() {
  auto bounds = getLocalBounds();
  group.setBounds(bounds);
  static const int spacing = 20;

  bounds.reduce(spacing * 1.25f, spacing * 1.25f);
  bounds.setWidth((bounds.getWidth() - 3 * spacing) / 4);
  attackSlider.setBounds(bounds);

  bounds.setX(bounds.getRight() + spacing);
  decaySlider.setBounds(bounds);

  bounds.setX(bounds.getRight() + spacing);
  sustainSlider.setBounds(bounds);

  bounds.setX(bounds.getRight() + spacing);
  releaseSlider.setBounds(bounds);
}
