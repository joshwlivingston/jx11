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

  filterResoKnob.label = "Reso";
  addAndMakeVisible(filterResoKnob);

  ampAttackSlider.label = "Amp Attack";
  addAndMakeVisible(ampAttackSlider);

  polyModeButton.setButtonText("Poly");
  polyModeButton.setClickingTogglesState(true);
  addAndMakeVisible(polyModeButton);

  juce::LookAndFeel::setDefaultLookAndFeel(&globalLNF);

  setSize(600, 400);
}

JX11AudioProcessorEditor::~JX11AudioProcessorEditor() {}

//==============================================================================
void JX11AudioProcessorEditor::paint(juce::Graphics &g) {
  g.fillAll(
      getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));
}

void JX11AudioProcessorEditor::resized() {
  juce::Rectangle r(20, 20, 75, 150);
  ampAttackSlider.setBounds(r);

  r = r.withY(r.getBottom() + 20).withWidth(100).withHeight(120);
  outputLevelKnob.setBounds(r);

  r = r.withX(r.getRight() + 20);
  filterResoKnob.setBounds(r);

  polyModeButton.setSize(80, 30);
  polyModeButton.setCentrePosition(r.withX(r.getRight()).getCentre());
}
