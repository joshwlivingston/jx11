#include "VibratoGroup.h"
#include "Skin.h"
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
  group.setBounds(getLocalBounds());

  // Two knobs stacked in a narrow column.
  static const int spacing = 8;
  auto content = Skin::groupContent(getLocalBounds());
  lfoRateKnob.setBounds(
      content.removeFromTop((content.getHeight() - spacing) / 2));
  content.removeFromTop(spacing);
  vibratoKnob.setBounds(content);
}
