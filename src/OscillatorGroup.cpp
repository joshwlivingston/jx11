#include "OscillatorGroup.h"
#include "juce_graphics/juce_graphics.h"

OscillatorGroup::OscillatorGroup(juce::String groupLabel, RotaryKnob &mix,
                                 RotaryKnob &tune, RotaryKnob &fine)
    : label(groupLabel), mixKnob(mix), tuneKnob(tune), fineKnob(fine) {
  group.setText(label);
  group.setTextLabelPosition(juce::Justification::top);
  addAndMakeVisible(group);

  mixKnob.label = "Mix";
  addAndMakeVisible(mixKnob);

  tuneKnob.label = "Tune";
  addAndMakeVisible(tuneKnob);

  fineKnob.label = "Fine";
  addAndMakeVisible(fineKnob);
}

OscillatorGroup::~OscillatorGroup() {}

void OscillatorGroup::resized() {
  auto bounds = getLocalBounds();
  group.setBounds(bounds);

  static const int spacing = 18;
  bounds.reduce(spacing * 1.25f, spacing * 1.25f);

  // Calculate dimensions based on 3 columns and 1 rows
  const int itemWidth = (bounds.getWidth() - 2 * spacing) / 3;
  const int itemHeight = (bounds.getHeight() - spacing);

  // Top Row (three knobs)
  auto topRow = bounds.removeFromTop(itemHeight);
  mixKnob.setBounds(topRow.removeFromLeft(itemWidth));
  topRow.removeFromLeft(spacing);
  tuneKnob.setBounds(topRow.removeFromLeft(itemWidth));
  topRow.removeFromLeft(spacing);
  fineKnob.setBounds(topRow.removeFromLeft(itemWidth));
}
