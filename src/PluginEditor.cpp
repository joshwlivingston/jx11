/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginEditor.h"
#include "PluginProcessor.h"
#include "juce_gui_basics/juce_gui_basics.h"

//==============================================================================
JX11AudioProcessorEditor::JX11AudioProcessorEditor(JX11AudioProcessor &p)
    : AudioProcessorEditor(&p), audioProcessor(p) {
  outputLevelKnob.label = "Level";
  addAndMakeVisible(outputLevelKnob);

  addAndMakeVisible(ampEnvGroup);
  addAndMakeVisible(filterEnvGroup);

  addAndMakeVisible(filterKnobGroup);
  addAndMakeVisible(oscillatorGroup);
  addAndMakeVisible(vibratoGroup);

  polyModeButton.setButtonText("Poly");
  polyModeButton.setClickingTogglesState(true);
  addAndMakeVisible(polyModeButton);

  setLookAndFeel(&globalLNF);

  setResizable(true, true);
  setSize(1000, 800);
}

JX11AudioProcessorEditor::~JX11AudioProcessorEditor() {}

//==============================================================================
void JX11AudioProcessorEditor::paint(juce::Graphics &g) {
  g.fillAll(
      getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));
}

void JX11AudioProcessorEditor::resized() {
  auto r = getLocalBounds();

  r.setX(20);
  r.setY(20);
  r.setWidth((r.getWidth() - 80) / 2);
  r.setHeight((r.getHeight() - 80) / 2);
  filterEnvGroup.setBounds(r);

  r.setX(r.getRight() + 40);
  ampEnvGroup.setBounds(r);

  r.setX(20);
  r.setY(r.getBottom() + 20);
  r.setWidth(450);
  r.setHeight(350);
  filterKnobGroup.setBounds(r);

  r.setX(r.getRight() + 40);
  r.setHeight(r.getHeight() / 2 + 40);
  oscillatorGroup.setBounds(r);

  r.setY(r.getBottom() + 20);
  r.setX(r.getX() + 75);
  r.setWidth(300);
  vibratoGroup.setBounds(r);

  r.setX(r.getRight() + 20);
  r.setWidth(100);
  r.setHeight(120);
  outputLevelKnob.setBounds(r);

  r.setX(r.getRight() + 20);
  polyModeButton.setSize(80, 30);
  polyModeButton.setCentrePosition(r.getCentre());
}
