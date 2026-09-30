#include "FilterKnobGroup.h"
#include "Skin.h"
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
  group.setBounds(getLocalBounds());

  // One row of five knobs, in signal-flow order.
  auto content = Skin::groupContent(getLocalBounds());
  const int columnWidth = content.getWidth() / 5;

  freqKnob.setBounds(content.removeFromLeft(columnWidth));
  resoKnob.setBounds(content.removeFromLeft(columnWidth));
  envKnob.setBounds(content.removeFromLeft(columnWidth));
  lfoKnob.setBounds(content.removeFromLeft(columnWidth));
  veloKnob.setBounds(content);
}
