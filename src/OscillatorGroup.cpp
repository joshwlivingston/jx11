#include "OscillatorGroup.h"
#include "Skin.h"
#include "juce_graphics/juce_graphics.h"

OscillatorGroup::OscillatorGroup(juce::String groupLabel, RotaryKnob &mix,
                                 RotaryKnob &tune, RotaryKnob &fine,
                                 RotaryKnob &noise)
    : label(groupLabel), mixKnob(mix), tuneKnob(tune), fineKnob(fine),
      noiseKnob(noise) {
  group.setText(label);
  group.setTextLabelPosition(juce::Justification::top);
  addAndMakeVisible(group);

  mixKnob.label = "Mix";
  addAndMakeVisible(mixKnob);

  tuneKnob.label = "Tune";
  addAndMakeVisible(tuneKnob);

  fineKnob.label = "Fine";
  addAndMakeVisible(fineKnob);

  noiseKnob.label = "Noise";
  addAndMakeVisible(noiseKnob);
}

OscillatorGroup::~OscillatorGroup() {}

void OscillatorGroup::resized() {
  group.setBounds(getLocalBounds());

  // One row of four knobs.
  auto content = Skin::groupContent(getLocalBounds());
  const int columnWidth = content.getWidth() / 4;

  mixKnob.setBounds(content.removeFromLeft(columnWidth));
  tuneKnob.setBounds(content.removeFromLeft(columnWidth));
  fineKnob.setBounds(content.removeFromLeft(columnWidth));
  noiseKnob.setBounds(content);
}
