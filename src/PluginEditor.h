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
#include "GlideGroup.h"
#include "IOSSwitch.h"
#include "LookAndFeel.h"
#include "OscillatorGroup.h"
#include "PluginProcessor.h"
#include "PresetDisplay.h"
#include "RotaryKnob.h"
#include "SegmentedControl.h"
#include "SynthPanel.h"
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

  // Every control lives on this panel, which the editor scales as a whole.
  // Declared before the controls so it outlives them.
  SynthPanel panel;

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

  EnvelopeGroup ampEnvGroup{"Amp Envelope", ampAttackSlider, ampDecaySlider,
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

  EnvelopeGroup filterEnvGroup{"Filter Envelope", filterAttackSlider,
                               filterDecaySlider, filterSustainSlider,
                               filterReleaseSlider};

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

  FilterKnobGroup filterKnobGroup{"Filter",           filterFreqKnob,
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

  RotaryKnob noiseKnob;
  SliderAttachment noiseAttachment{
      audioProcessor.apvts, ParameterID::noise.getParamID(), noiseKnob.slider};

  OscillatorGroup oscillatorGroup{"Oscillator", oscMixKnob, oscTuneKnob,
                                  oscFineKnob, noiseKnob};

  RotaryKnob vibratoKnob;
  SliderAttachment vibratoAttachment{audioProcessor.apvts,
                                     ParameterID::vibrato.getParamID(),
                                     vibratoKnob.slider};

  RotaryKnob lfoRateKnob;
  SliderAttachment lfoRateAttachment{audioProcessor.apvts,
                                     ParameterID::lfoRate.getParamID(),
                                     lfoRateKnob.slider};

  VibratoGroup vibratoGroup{"LFO", lfoRateKnob, vibratoKnob};

  // Segmented controls attach to their parameter themselves.
  SegmentedControl glideModeSelector{
      *audioProcessor.apvts.getParameter(ParameterID::glideMode.getParamID()),
      {"Off", "Legato", "Always"}};

  RotaryKnob glideRateKnob;
  SliderAttachment glideRateAttachment{audioProcessor.apvts,
                                       ParameterID::glideRate.getParamID(),
                                       glideRateKnob.slider};

  RotaryKnob glideBendKnob;
  SliderAttachment glideBendAttachment{audioProcessor.apvts,
                                       ParameterID::glideBend.getParamID(),
                                       glideBendKnob.slider};

  GlideGroup glideGroup{"Glide", glideModeSelector, glideRateKnob,
                        glideBendKnob};

  // Header: presets, voice mode, octave, master tuning and level.
  PresetDisplay presetDisplay{audioProcessor};

  IOSSwitch polyModeSwitch{"POLY", "MONO"};
  ButtonAttachment polyModeAttachment{
      audioProcessor.apvts, ParameterID::polyMode.getParamID(), polyModeSwitch};

  SegmentedControl octaveSelector{
      *audioProcessor.apvts.getParameter(ParameterID::octave.getParamID()),
      {"-2", "-1", "0", "+1", "+2"}};

  RotaryKnob tuningKnob;
  SliderAttachment tuningAttachment{audioProcessor.apvts,
                                    ParameterID::tuning.getParamID(),
                                    tuningKnob.slider};

  // Double-clicking a knob or fader returns it to its parameter's default.
  void setDefaultValue(JX11Slider &control, const juce::ParameterID &id);

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(JX11AudioProcessorEditor)
};
