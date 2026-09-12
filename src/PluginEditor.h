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

#include "LookAndFeel.h"
#include "PluginProcessor.h"
#include "RotaryKnob.h"
#include "VerticalSlider.h"

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

  RotaryKnob filterResoKnob;
  SliderAttachment filterResoAttachment{audioProcessor.apvts,
                                        ParameterID::filterReso.getParamID(),
                                        filterResoKnob.slider};

  VerticalSlider ampAttackSlider;
  SliderAttachment ampAttackAttachment{audioProcessor.apvts,
                                       ParameterID::envAttack.getParamID(),
                                       ampAttackSlider.slider};

  juce::TextButton polyModeButton;
  ButtonAttachment polyModeAttachment{
      audioProcessor.apvts, ParameterID::polyMode.getParamID(), polyModeButton};

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(JX11AudioProcessorEditor)
};
