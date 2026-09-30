#include "EnvelopeGroup.h"
#include "Skin.h"
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
  group.setBounds(getLocalBounds());

  // Four equal columns of faders.
  auto content = Skin::groupContent(getLocalBounds());
  const int columnWidth = content.getWidth() / 4;

  attackSlider.setBounds(content.removeFromLeft(columnWidth));
  decaySlider.setBounds(content.removeFromLeft(columnWidth));
  sustainSlider.setBounds(content.removeFromLeft(columnWidth));
  releaseSlider.setBounds(content);
}
