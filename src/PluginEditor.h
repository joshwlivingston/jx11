/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <juce_audio_basics/juce_audio_basics.h>
#include <juce_audio_processors/juce_audio_processors.h>
#include <juce_core/juce_core.h>
#include <juce_data_structures/juce_data_structures.h>
#include <juce_graphics/juce_graphics.h>
#include <juce_gui_basics/juce_gui_basics.h>

#include "EnvelopeGroup.h"
#include "FilterKnobGroup.h"
#include "LookAndFeel.h"
#include "OscillatorGroup.h"
#include "PluginProcessor.h"
#include "RotaryKnob.h"
#include "VerticalSlider.h"
#include "VibratoGroup.h"

//==============================================================================
/**
 */
class JX11AudioProcessorEditor : public juce::AudioProcessorEditor {
public:
  JX11AudioProcessorEditor(JX11AudioProcessor &);
  ~JX11AudioProcessorEditor() override;

  //==============================================================================
  void paint(juce::Graphics &) override;
  void resized() override;

private:
  // This reference is provided as a quick way for your editor to
  // access the processor object that created it.
  JX11AudioProcessor &audioProcessor;

  LookAndFeel globalLNF;

  using APVTS = juce::AudioProcessorValueTreeState;
  using SliderAttachment = APVTS::SliderAttachment;
  using ButtonAttachment = APVTS::ButtonAttachment;

  // attachments must be declared after the ui they are attached to
  // why? becuase JUCE's garbage collector destroys objects in the class from
  // bottom up
  RotaryKnob outputLevelKnob;
  SliderAttachment outputLevelAttachment{audioProcessor.apvts,
                                         ParameterID::outputLevel.getParamID(),
                                         outputLevelKnob.slider};

  VerticalSlider ampAttackSlider;
  SliderAttachment ampAttackAttachment{audioProcessor.apvts,
                                       ParameterID::envAttack.getParamID(),
                                       ampAttackSlider.slider};

  VerticalSlider ampDecaySlider;
  SliderAttachment ampDecayAttachment{audioProcessor.apvts,
                                      ParameterID::envDecay.getParamID(),
                                      ampDecaySlider.slider};

  VerticalSlider ampSustainSlider;
  SliderAttachment ampSustainAttachment{audioProcessor.apvts,
                                        ParameterID::envSustain.getParamID(),
                                        ampSustainSlider.slider};

  VerticalSlider ampReleaseSlider;
  SliderAttachment ampReleaseAttachment{audioProcessor.apvts,
                                        ParameterID::envRelease.getParamID(),
                                        ampReleaseSlider.slider};

  EnvelopeGroup ampEnvGroup{"Amp", ampAttackSlider, ampDecaySlider,
                            ampSustainSlider, ampReleaseSlider};

  VerticalSlider filterAttackSlider;
  SliderAttachment filterAttackAttachment{
      audioProcessor.apvts, ParameterID::filterAttack.getParamID(),
      filterAttackSlider.slider};

  VerticalSlider filterDecaySlider;
  SliderAttachment filterDecayAttachment{audioProcessor.apvts,
                                         ParameterID::filterDecay.getParamID(),
                                         filterDecaySlider.slider};

  VerticalSlider filterSustainSlider;
  SliderAttachment filterSustainAttachment{
      audioProcessor.apvts, ParameterID::filterSustain.getParamID(),
      filterSustainSlider.slider};

  VerticalSlider filterReleaseSlider;
  SliderAttachment filterReleaseAttachment{
      audioProcessor.apvts, ParameterID::filterRelease.getParamID(),
      filterReleaseSlider.slider};

  EnvelopeGroup filterEnvGroup{"Filter", filterAttackSlider, filterDecaySlider,
                               filterSustainSlider, filterReleaseSlider};

  RotaryKnob filterFreqKnob;
  SliderAttachment filterFreqAttachment{audioProcessor.apvts,
                                        ParameterID::filterFreq.getParamID(),
                                        filterFreqKnob.slider};

  RotaryKnob filterEnvKnob;
  SliderAttachment filterEnvAttachment{audioProcessor.apvts,
                                       ParameterID::filterEnv.getParamID(),
                                       filterEnvKnob.slider};

  RotaryKnob filterResoKnob;
  SliderAttachment filterResoAttachment{audioProcessor.apvts,
                                        ParameterID::filterReso.getParamID(),
                                        filterResoKnob.slider};

  RotaryKnob filterLFOKnob;
  SliderAttachment filterLFOAttachment{audioProcessor.apvts,
                                       ParameterID::filterLFO.getParamID(),
                                       filterLFOKnob.slider};

  RotaryKnob filterVelocityKnob;
  SliderAttachment filterVelocityAttachment{
      audioProcessor.apvts, ParameterID::filterVelocity.getParamID(),
      filterVelocityKnob.slider};

  FilterKnobGroup filterKnobGroup{"Filter Controls",  filterFreqKnob,
                                  filterVelocityKnob, filterResoKnob,
                                  filterEnvKnob,      filterLFOKnob};

  RotaryKnob oscMixKnob;
  SliderAttachment oscMixAttachment{audioProcessor.apvts,
                                    ParameterID::oscMix.getParamID(),
                                    oscMixKnob.slider};

  RotaryKnob oscTuneKnob;
  SliderAttachment oscTuneAttachment{audioProcessor.apvts,
                                     ParameterID::oscTune.getParamID(),
                                     oscTuneKnob.slider};

  RotaryKnob oscFineKnob;
  SliderAttachment oscFineAttachment{audioProcessor.apvts,
                                     ParameterID::oscFine.getParamID(),
                                     oscFineKnob.slider};

  OscillatorGroup oscillatorGroup{"Oscillator", oscMixKnob, oscTuneKnob,
                                  oscFineKnob};

  RotaryKnob vibratoKnob;
  SliderAttachment vibratoAttachment{audioProcessor.apvts,
                                     ParameterID::vibrato.getParamID(),
                                     vibratoKnob.slider};

  RotaryKnob lfoRateKnob;
  SliderAttachment lfoRateAttachment{audioProcessor.apvts,
                                     ParameterID::lfoRate.getParamID(),
                                     lfoRateKnob.slider};

  VibratoGroup vibratoGroup{"Vibrato", lfoRateKnob, vibratoKnob};

  juce::TextButton polyModeButton;
  ButtonAttachment polyModeAttachment{
      audioProcessor.apvts, ParameterID::polyMode.getParamID(), polyModeButton};

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(JX11AudioProcessorEditor)
};
