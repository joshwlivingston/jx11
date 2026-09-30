#include "GlideGroup.h"
#include "Skin.h"
#include "juce_graphics/juce_graphics.h"
#include "juce_gui_basics/juce_gui_basics.h"

GlideGroup::GlideGroup(juce::String groupLabel, SegmentedControl &glideMode,
                       RotaryKnob &glideRate, RotaryKnob &glideBend)
    : label(groupLabel), glideModeSelector(glideMode), glideRateKnob(glideRate),
      glideBendKnob(glideBend) {
  group.setText(label);
  group.setTextLabelPosition(juce::Justification::top);
  addAndMakeVisible(group);

  addAndMakeVisible(glideModeSelector);

  glideRateKnob.label = "Rate";
  addAndMakeVisible(glideRateKnob);

  glideBendKnob.label = "Bend";
  addAndMakeVisible(glideBendKnob);
}

GlideGroup::~GlideGroup() {}

void GlideGroup::resized() {
  group.setBounds(getLocalBounds());

  // Mode selector across the top, both knobs side by side beneath it.
  auto content = Skin::groupContent(getLocalBounds());
  glideModeSelector.setBounds(
      content.removeFromTop(30).reduced(content.getWidth() / 12, 0));
  content.removeFromTop(12);

  glideRateKnob.setBounds(content.removeFromLeft(content.getWidth() / 2));
  glideBendKnob.setBounds(content);
}
