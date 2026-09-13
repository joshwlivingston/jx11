#include "FilterKnobGroup.h"
#include "juce_graphics/juce_graphics.h"

FilterKnobGroup::FilterKnobGroup(juce::String groupLabel, RotaryKnob &freq,
                                 RotaryKnob &velo, RotaryKnob &reso,
                                 RotaryKnob &env, RotaryKnob &lfo)
    : label(groupLabel), freqKnob(freq), veloKnob(velo), resoKnob(reso),
      envKnob(env), lfoKnob(lfo) {
  group.setText(label);
  group.setTextLabelPosition(juce::Justification::top);
  addAndMakeVisible(group);

  freqKnob.label = "Freq";
  addAndMakeVisible(freqKnob);

  veloKnob.label = "Velocity";
  addAndMakeVisible(veloKnob);

  resoKnob.label = "Resonance";
  addAndMakeVisible(resoKnob);

  envKnob.label = "Env";
  addAndMakeVisible(envKnob);

  lfoKnob.label = "LFO";
  addAndMakeVisible(lfoKnob);
}

FilterKnobGroup::~FilterKnobGroup() {}

void FilterKnobGroup::resized() {
  auto bounds = getLocalBounds();
  group.setBounds(bounds);

  static const int spacing = 18;
  bounds.reduce(spacing * 1.25f, spacing * 1.25f);

  // Calculate dimensions based on 3 columns and 2 rows
  const int itemWidth = (bounds.getWidth() - 2 * spacing) / 3;
  const int itemHeight = (bounds.getHeight() - spacing) / 2;

  // Top Row (three knobs)
  auto topRow = bounds.removeFromTop(itemHeight);
  freqKnob.setBounds(topRow.removeFromLeft(itemWidth));
  topRow.removeFromLeft(spacing);
  resoKnob.setBounds(topRow.removeFromLeft(itemWidth));
  topRow.removeFromLeft(spacing);
  veloKnob.setBounds(topRow.removeFromLeft(itemWidth));

  // Bottom Row (two knobs)
  // Adds margin to left and right edges of bottom row - preserves same spacing
  // between knobs
  auto bottomRow = bounds.removeFromTop(itemHeight + spacing);
  const int bottomRowOffset = (itemWidth / 2) + (spacing / 2);
  bottomRow.removeFromLeft(bottomRowOffset);
  envKnob.setBounds(bottomRow.removeFromLeft(itemWidth));
  bottomRow.removeFromLeft(spacing);
  lfoKnob.setBounds(bottomRow.removeFromLeft(itemWidth));
}
