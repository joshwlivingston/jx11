/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.
    All communication between the plugin and the DAW happens through this
    object.

    "Writing a plugin is a matter of implementing these methods."

    Lifecycle of a plugin:

    1. The DAW constructs the plugin.
    2. The plugin's properties are inspected. These properties are determined by
       member functions acceptsMidi(), producesMidi(), getNumPrograms(), etc.
    3. prepareToPlay() is called, allocating memory, resetting state, and
       receiving information such as the sample rate and audio buffer size.
       Note: memory allocation happens any number of times during the plugin's
       lifecycle, so no resource allocation actually occurs on construction.
    4. releaseResources() in the counterpart to prepareToPlay(), freeing
       memory. Note: resource deallocation occurs at this step, not at
       destuction.
    5. processBlock() is the core method, called hundreds or thousands of times
       per second. It servers as the workhorse of the plugin.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/**
*/
// All juce functions and classes are in the juce namespace
class JX11AudioProcessor  : public juce::AudioProcessor
{
public:
    //==========================================================================
  JX11AudioProcessor();
  ~JX11AudioProcessor() override;

  //==========================================================================
  void prepareToPlay (double sampleRate, int samplesPerBlock) override;
  void releaseResources() override;

  #ifndef JucePlugin_PreferredChannelConfigurations
  bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
  #endif

  void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

  //==========================================================================
  juce::AudioProcessorEditor* createEditor() override;
  bool hasEditor() const override;

  //==========================================================================
  const juce::String getName() const override;

  bool acceptsMidi() const override;
  bool producesMidi() const override;
  bool isMidiEffect() const override;
  double getTailLengthSeconds() const override;

  //==========================================================================
  int getNumPrograms() override;
  int getCurrentProgram() override;
  void setCurrentProgram (int index) override;
  const juce::String getProgramName (int index) override;
  void changeProgramName (int index, const juce::String& newName) override;

  //==========================================================================
  void getStateInformation (juce::MemoryBlock& destData) override;
  void setStateInformation (const void* data, int sizeInBytes) override;

private:
  //==========================================================================
  // disables copy constructor for the class and enable JUCE's memory leak
  // detector
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (JX11AudioProcessor)
  
  void splitBufferByEvents(juce::AudioBuffer<float> &buffer,
                           juce::MidiBuffer& midiMessages);
  void handleMIDI(uint8_t data0, uint8_t data1, uint8_t data2);
  void render(juce::AudioBuffer<float>& buffer,
              int sampleCount, int bufferOffset);
};
