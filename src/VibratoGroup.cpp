#include "VibratoGroup.h"
#include "juce_graphics/juce_graphics.h"

VibratoGroup::VibratoGroup(juce::String groupLabel, RotaryKnob &rate,
                           RotaryKnob &vibrato)
    : label(groupLabel), lfoRateKnob(rate), vibratoKnob(vibrato) {
  group.setText(label);
  group.setTextLabelPosition(juce::Justification::top);
  addAndMakeVisible(group);

  lfoRateKnob.label = "Rate";
  addAndMakeVisible(lfoRateKnob);

  vibratoKnob.label = "Amount";
  addAndMakeVisible(vibratoKnob);
}

VibratoGroup::~VibratoGroup() {}

void VibratoGroup::resized() {
  auto bounds = getLocalBounds();
  group.setBounds(bounds);

  static const int spacing = 18;
  bounds.reduce(spacing * 1.25f, spacing * 1.25f);

  // Calculate dimensions based on 2 columns and 1 rows
  const int itemWidth = (bounds.getWidth() - 1 * spacing) / 2;
  const int itemHeight = (bounds.getHeight() - spacing);

  // Top Row (three knobs)
  auto topRow = bounds.removeFromTop(itemHeight);
  lfoRateKnob.setBounds(topRow.removeFromLeft(itemWidth));
  topRow.removeFromLeft(spacing);
  vibratoKnob.setBounds(topRow.removeFromLeft(itemWidth));
}
