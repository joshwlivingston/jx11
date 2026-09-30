#include "GlideGroup.h"
#include "juce_graphics/juce_graphics.h"
#include "juce_gui_basics/juce_gui_basics.h"

GlideGroup::GlideGroup(juce::String groupLabel, juce::ComboBox &glideMode,
                       RotaryKnob &glideRate, RotaryKnob &glideBend)
    : label(groupLabel), glideModeBox(glideMode), glideRateKnob(glideRate),
      glideBendKnob(glideBend) {
  group.setText(label);
  group.setTextLabelPosition(juce::Justification::top);
  addAndMakeVisible(group);

  glideModeBox.setText("Mode");
  glideModeBox.addItem("Off", 1);
  glideModeBox.addItem("Legato", 2);
  glideModeBox.addItem("Always", 3);
  addAndMakeVisible(glideModeBox);

  glideRateKnob.label = "Rate";
  addAndMakeVisible(glideRateKnob);

  glideBendKnob.label = "Bend";
  addAndMakeVisible(glideBendKnob);
}

GlideGroup::~GlideGroup() {}

void GlideGroup::resized() {
  auto bounds = getLocalBounds();
  group.setBounds(bounds);

  static const int spacing = 18;
  bounds.reduce(spacing * 1.25f, spacing * 1.25f);

  // glideMode centered on bottom, both knobs on top
  const int boxWidth = bounds.getWidth() / 3;
  const int knobWidth = (bounds.getWidth() - spacing) / 2;

  // 2:1 knob:button height
  const int boxHeight = (bounds.getHeight() - spacing) / 3;
  const int knobHeight = boxHeight * 2;

  // glideRateKnob
  bounds.setWidth(knobWidth);
  bounds.setHeight(knobHeight);
  glideRateKnob.setBounds(bounds);

  // glideBendKnob
  bounds.setX(bounds.getRight() + spacing);
  glideBendKnob.setBounds(bounds);

  // glideModeBox
  bounds.setX(boxWidth + spacing * 1.25);
  bounds.setY(bounds.getBottom() + spacing);
  bounds.setHeight(boxHeight);
  bounds.setWidth(boxWidth);
  glideModeBox.setBounds(bounds);
}
