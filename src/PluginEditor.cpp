/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginEditor.h"
#include "PluginProcessor.h"
#include "Skin.h"
#include "juce_gui_basics/juce_gui_basics.h"

//==============================================================================
JX11AudioProcessorEditor::JX11AudioProcessorEditor(JX11AudioProcessor &p)
    : AudioProcessorEditor(&p), audioProcessor(p) {
  addAndMakeVisible(panel);

  panel.addAndMakeVisible(ampEnvGroup);
  panel.addAndMakeVisible(filterEnvGroup);
  panel.addAndMakeVisible(filterKnobGroup);
  panel.addAndMakeVisible(oscillatorGroup);
  panel.addAndMakeVisible(vibratoGroup);
  panel.addAndMakeVisible(glideGroup);

  panel.addAndMakeVisible(presetDisplay);

  panel.addAndMakeVisible(polyModeSwitch);
  panel.addCaption(polyModeSwitch, "Voice");

  panel.addAndMakeVisible(octaveSelector);
  panel.addCaption(octaveSelector, "Octave");

  // The header sits on dark leather, so its knobs use light ink.
  tuningKnob.label = "Tune";
  tuningKnob.setInkColour(Skin::inkOnLeather);
  panel.addAndMakeVisible(tuningKnob);

  outputLevelKnob.label = "Level";
  outputLevelKnob.setInkColour(Skin::inkOnLeather);
  panel.addAndMakeVisible(outputLevelKnob);

  setDefaultValue(outputLevelKnob, ParameterID::outputLevel);
  setDefaultValue(tuningKnob, ParameterID::tuning);
  setDefaultValue(ampAttackSlider, ParameterID::envAttack);
  setDefaultValue(ampDecaySlider, ParameterID::envDecay);
  setDefaultValue(ampSustainSlider, ParameterID::envSustain);
  setDefaultValue(ampReleaseSlider, ParameterID::envRelease);
  setDefaultValue(filterAttackSlider, ParameterID::filterAttack);
  setDefaultValue(filterDecaySlider, ParameterID::filterDecay);
  setDefaultValue(filterSustainSlider, ParameterID::filterSustain);
  setDefaultValue(filterReleaseSlider, ParameterID::filterRelease);
  setDefaultValue(filterFreqKnob, ParameterID::filterFreq);
  setDefaultValue(filterResoKnob, ParameterID::filterReso);
  setDefaultValue(filterEnvKnob, ParameterID::filterEnv);
  setDefaultValue(filterLFOKnob, ParameterID::filterLFO);
  setDefaultValue(filterVelocityKnob, ParameterID::filterVelocity);
  setDefaultValue(oscMixKnob, ParameterID::oscMix);
  setDefaultValue(oscTuneKnob, ParameterID::oscTune);
  setDefaultValue(oscFineKnob, ParameterID::oscFine);
  setDefaultValue(noiseKnob, ParameterID::noise);
  setDefaultValue(lfoRateKnob, ParameterID::lfoRate);
  setDefaultValue(vibratoKnob, ParameterID::vibrato);
  setDefaultValue(glideRateKnob, ParameterID::glideRate);
  setDefaultValue(glideBendKnob, ParameterID::glideBend);

  // Set once the hierarchy exists, so the change reaches every child and the
  // sliders rebuild their value boxes with this look.
  setLookAndFeel(&globalLNF);

  // The artwork is drawn for one canvas; the window keeps its proportions and
  // scales everything together.
  setResizable(true, true);
  setResizeLimits(Skin::baseWidth * 3 / 4, Skin::baseHeight * 3 / 4,
                  Skin::baseWidth * 2, Skin::baseHeight * 2);
  getConstrainer()->setFixedAspectRatio(double(Skin::baseWidth) /
                                        double(Skin::baseHeight));
  setSize(Skin::baseWidth, Skin::baseHeight);
}

JX11AudioProcessorEditor::~JX11AudioProcessorEditor() {
  setLookAndFeel(nullptr);
}

void JX11AudioProcessorEditor::setDefaultValue(JX11Slider &control,
                                               const juce::ParameterID &id) {
  if (auto *param = audioProcessor.apvts.getParameter(id.getParamID()))
    control.setDefaultValue(param->convertFrom0to1(param->getDefaultValue()));
}

//==============================================================================
void JX11AudioProcessorEditor::paint(juce::Graphics &g) {
  // The panel covers the whole editor; this only shows during a live resize.
  g.fillAll(Skin::leather);
}

void JX11AudioProcessorEditor::resized() {
  // Fit and centre, in case a host forces a size that ignores the aspect
  // ratio.
  const float scale = juce::jmin(float(getWidth()) / float(Skin::baseWidth),
                                 float(getHeight()) / float(Skin::baseHeight));
  panel.setTransform(juce::AffineTransform::scale(scale).translated(
      (float(getWidth()) - float(Skin::baseWidth) * scale) * 0.5f,
      (float(getHeight()) - float(Skin::baseHeight) * scale) * 0.5f));

  // Header (base canvas coordinates). The logo is painted by the panel.
  presetDisplay.setBounds(236, 28, 360, 62);
  polyModeSwitch.setBounds(618, 50, 92, 30);
  octaveSelector.setBounds(734, 50, 180, 30);
  tuningKnob.setBounds(926, 14, 64, 92);
  outputLevelKnob.setBounds(994, 14, 64, 92);

  // Faceplate: sound sources and filter on top, envelopes and modulation
  // below.
  const int left = Skin::cheekWidth + 24;
  const int width = Skin::baseWidth - 2 * left;
  const int gap = 16;

  auto topRow = juce::Rectangle<int>(left, Skin::headerHeight + 16, width, 190);
  oscillatorGroup.setBounds(topRow.removeFromLeft((width - gap) * 4 / 9));
  topRow.removeFromLeft(gap);
  filterKnobGroup.setBounds(topRow);

  auto bottomRow =
      juce::Rectangle<int>(left, topRow.getBottom() + 14, width,
                           Skin::baseHeight - topRow.getBottom() - 14 - 18);
  ampEnvGroup.setBounds(bottomRow.removeFromLeft(252));
  bottomRow.removeFromLeft(gap);
  filterEnvGroup.setBounds(bottomRow.removeFromLeft(252));
  bottomRow.removeFromLeft(gap);
  vibratoGroup.setBounds(bottomRow.removeFromLeft(150));
  bottomRow.removeFromLeft(gap);
  glideGroup.setBounds(bottomRow);
}
