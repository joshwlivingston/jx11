/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "juce_audio_processors/juce_audio_processors.h"
#include "juce_core/juce_core.h"

//==============================================================================
JX11AudioProcessor::JX11AudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
    : AudioProcessor(
          BusesProperties()
#if !JucePlugin_IsMidiEffect
#if !JucePlugin_IsSynth
              .withInput("Input", juce::AudioChannelSet::stereo(), true)
#endif
              // creates a BusesProperties() object with stereo output bus named
              // "Output"
              .withOutput("Output", juce::AudioChannelSet::stereo(), true)
#endif
      )
#endif // if blocks correspond to settings in Projucer
{
}

JX11AudioProcessor::~JX11AudioProcessor() {}

//==============================================================================
const juce::String JX11AudioProcessor::getName() const {
  return JucePlugin_Name;
}

bool JX11AudioProcessor::acceptsMidi() const {
#if JucePlugin_WantsMidiInput
  return true;
#else
  return false;
#endif
}

bool JX11AudioProcessor::producesMidi() const {
#if JucePlugin_ProducesMidiOutput
  return true;
#else
  return false;
#endif
}

bool JX11AudioProcessor::isMidiEffect() const {
#if JucePlugin_IsMidiEffect
  return true;
#else
  return false;
#endif
}

double JX11AudioProcessor::getTailLengthSeconds() const { return 0.0; }

int JX11AudioProcessor::getNumPrograms() {
  return 1; // NB: some hosts don't cope very well if you tell them there are 0
            // programs, so this should be at least 1, even if you're not really
            // implementing programs.
}

int JX11AudioProcessor::getCurrentProgram() { return 0; }

void JX11AudioProcessor::setCurrentProgram(int index) {}

const juce::String JX11AudioProcessor::getProgramName(int index) { return {}; }

void JX11AudioProcessor::changeProgramName(int index,
                                           const juce::String &newName) {}

//==============================================================================

// prepareToPlay() is called before the host start using the plugin; it
// communicates the current sample rate and the maximum block size to expect.
void JX11AudioProcessor::prepareToPlay(double sampleRate, int samplesPerBlock) {
  synth.allocateResources(sampleRate, samplesPerBlock);
  reset();
}

void JX11AudioProcessor::releaseResources() { synth.deallocateResources(); }

void JX11AudioProcessor::reset() { synth.reset(); }

#ifndef JucePlugin_PreferredChannelConfigurations
bool JX11AudioProcessor::isBusesLayoutSupported(
    const BusesLayout &layouts) const {
#if JucePlugin_IsMidiEffect
  juce::ignoreUnused(layouts);
  return true;
#endif
  // Communicates to the host that the plug-in supports both mono and stereo
  // To support stereo only, remove the inequality for mono()
  if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono() &&
      layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
    return false;

// This checks if the input layout matches the output layout
#if !JucePlugin_IsSynth
  if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
    return false;
#endif

  return true;
}
#endif

// Audio callback method. Called once per block.
void JX11AudioProcessor::processBlock(
    juce::AudioBuffer<float> &buffer, // Where to place outgoing MIDI messages
    juce::MidiBuffer &midiMessages)   // Contains incoming MIDI messages
{
  // Sets a CPU flag that automatically truncates small floats to 0, instead of
  // converting them to denormals, which are much slower than regular floats
  juce::ScopedNoDenormals noDenormals;

  const auto totalNumInputChannels = getTotalNumInputChannels();
  const auto totalNumOutputChannels = getTotalNumOutputChannels();

  // JX11 does not have an audio input channel, and JUCE does not guarantee that
  // the AudioBuffer channel will be empty. So, it's good defensive practice to
  // ensure the incoming audio buffer contains only silence.
  for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i) {
    buffer.clear(i, 0, buffer.getNumSamples());
  }

  splitBufferByEvents(buffer, midiMessages);
}

// Split Audio Buffer into smaller buffers based on MIDI event timings. Allows
// for MIDI events to be called at the beginning of all blocks.
void JX11AudioProcessor::splitBufferByEvents(juce::AudioBuffer<float> &buffer,
                                             juce::MidiBuffer &midiMessages) {
  int bufferOffset = 0;
  for (const auto metadata : midiMessages) {
    // Render audio before this event
    const int samplesThisSegment = metadata.samplePosition - bufferOffset;
    if (samplesThisSegment > 0) {
      render(buffer, samplesThisSegment, bufferOffset);
      bufferOffset += samplesThisSegment;
    }

    // Handle this event
    if (metadata.numBytes <= 3) {
      const uint8_t data1 = metadata.numBytes >= 2 ? metadata.data[1] : 0;
      const uint8_t data2 = metadata.numBytes == 3 ? metadata.data[2] : 0;
      handleMIDI(metadata.data[0], data1, data2);
    }
  }

  // Render all audio after the last MIDI event
  int samplesLastSegment = buffer.getNumSamples() - bufferOffset;
  if (samplesLastSegment > 0) {
    render(buffer, samplesLastSegment, bufferOffset);
  }

  midiMessages.clear();
}

void JX11AudioProcessor::handleMIDI(uint8_t status, uint8_t data0,
                                    uint8_t data1) {
  synth.midiMessage(status, data0, data1);
}

void JX11AudioProcessor::render(juce::AudioBuffer<float> &buffer,
                                int sampleCount, int bufferOffset) {
  float *outputBuffers[2] = {nullptr, nullptr};
  outputBuffers[0] = buffer.getWritePointer(0) + bufferOffset;
  if (getTotalNumOutputChannels() > 1) {
    outputBuffers[1] = buffer.getWritePointer(1) + bufferOffset;
  }
  synth.render(outputBuffers, sampleCount);
}

//==============================================================================
bool JX11AudioProcessor::hasEditor() const {
  return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor *JX11AudioProcessor::createEditor() {
  auto editor = new juce::GenericAudioProcessorEditor(*this);
  editor->setSize(500, 1050);
  return editor;
}

//==============================================================================
void JX11AudioProcessor::getStateInformation(juce::MemoryBlock &destData) {
  // You should use this method to store your parameters in the memory block.
  // You could do that either as raw data, or use the XML or ValueTree classes
  // as intermediaries to make it easy to save and load complex data.
}

void JX11AudioProcessor::setStateInformation(const void *data,
                                             int sizeInBytes) {
  // You should use this method to restore your parameters from this memory
  // block, whose contents will have been created by the getStateInformation()
  // call.
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor *JUCE_CALLTYPE createPluginFilter() {
  return new JX11AudioProcessor();
}

juce::AudioProcessorValueTreeState::ParameterLayout
JX11AudioProcessor::createParameterLayout() {
  juce::AudioProcessorValueTreeState::ParameterLayout parameters;

  // Mono/Poly
  parameters.add(std::make_unique<juce::AudioParameterChoice>(
      ParameterID::polyMode, "Polyphony", juce::StringArray{"Mono", "Poly"},
      1));

  // Oscillator tuning
  parameters.add(std::make_unique<juce::AudioParameterFloat>(
      ParameterID::oscTune, "Osc Tune",
      juce::NormalisableRange<float>(-24.0f, 24.0f, 1.0f), -12.0f,
      juce::AudioParameterFloatAttributes().withLabel("semi")));

  // Oscillator fine tuning
  // Skews toward smaller steps in the middle of the range, and larger slider
  // steps in the higher parts of the range
  parameters.add(std::make_unique<juce::AudioParameterFloat>(
      ParameterID::oscFine, "Osc Fine",
      juce::NormalisableRange<float>(-50.0f, 50.0f, 0.1f, 0.3f, true), 0.0f,
      juce::AudioParameterFloatAttributes().withLabel("cent")));

  // Oscillator mix
  auto oscMixStringFromValue = [](float value, int) {
    char s[16] = {0};
    snprintf(s, 16, "%4.0f:%2.0f", 100.0 - 0.5 * value, 0.5 * value);
    return juce::String(s);
  };

  parameters.add(std::make_unique<juce::AudioParameterFloat>(
      ParameterID::oscMix, "Osc Mix",
      juce::NormalisableRange<float>(0.0f, 100.0f), 0.0f,
      juce::AudioParameterFloatAttributes()
          .withLabel("%")
          .withStringFromValueFunction(oscMixStringFromValue)));

  // Glide mode
  parameters.add(std::make_unique<juce::AudioParameterChoice>(
      ParameterID::glideMode, "Glide Mode",
      juce::StringArray{"Off", "Legato", "Always"}, 0));

  // Glide rate
  parameters.add(std::make_unique<juce::AudioParameterFloat>(
      ParameterID::glideRate, "Glide Rate",
      juce::NormalisableRange<float>(0.0f, 100.0f, 1.0f), 35.0f,
      juce::AudioParameterFloatAttributes().withLabel("%")));

  // Glide bend
  parameters.add(std::make_unique<juce::AudioParameterFloat>(
      ParameterID::glideBend, "Glide Bend",
      juce::NormalisableRange<float>(-36.0f, 36.0f, 0.01f, 0.4f, true), 0.0f,
      juce::AudioParameterFloatAttributes().withLabel("semi")));

  // Filter frequency
  parameters.add(std::make_unique<juce::AudioParameterFloat>(
      ParameterID::filterFreq, "Filter Freq",
      juce::NormalisableRange<float>(0.0f, 100.0f, 0.1f), 100.0f,
      juce::AudioParameterFloatAttributes().withLabel("%")));

  // Filter resonance
  parameters.add(std::make_unique<juce::AudioParameterFloat>(
      ParameterID::filterReso, "Filter Reso",
      juce::NormalisableRange<float>(0.0f, 100.0f, 1.0f), 15.0f,
      juce::AudioParameterFloatAttributes().withLabel("%")));

  // Filter Envelope
  parameters.add(std::make_unique<juce::AudioParameterFloat>(
      ParameterID::filterEnv, "Filter Env",
      juce::NormalisableRange<float>(-100.0f, 100.0f, 0.1f), 50.0f,
      juce::AudioParameterFloatAttributes().withLabel("%")));

  // Filter LFO
  parameters.add(std::make_unique<juce::AudioParameterFloat>(
      ParameterID::filterLFO, "Filter LFO",
      juce::NormalisableRange<float>(0.0f, 100.0f, 1.0f), 0.0f,
      juce::AudioParameterFloatAttributes().withLabel("%")));

  // Filter Velocity
  auto filterVelocityStringFromValue = [](float value, int) {
    if (value < -90.0f) {
      return juce::String("OFF");
    } else {
      return juce::String(value);
    }
  };

  parameters.add(std::make_unique<juce::AudioParameterFloat>(
      ParameterID::filterVelocity, "Velocity",
      juce::NormalisableRange<float>(-100.0f, 100.0f, 1.0f), 0.0f,
      juce::AudioParameterFloatAttributes()
          .withLabel("%")
          .withStringFromValueFunction(filterVelocityStringFromValue)));

  // Filter Attack
  parameters.add(std::make_unique<juce::AudioParameterFloat>(
      ParameterID::filterAttack, "Filter Attack",
      juce::NormalisableRange<float>(0.0f, 100.0f, 1.0f), 0.0f,
      juce::AudioParameterFloatAttributes().withLabel("%")));

  // Filter Decay
  parameters.add(std::make_unique<juce::AudioParameterFloat>(
      ParameterID::filterDecay, "Filter Decay",
      juce::NormalisableRange<float>(0.0f, 100.0f, 1.0f), 30.0f,
      juce::AudioParameterFloatAttributes().withLabel("%")));

  // Filter Sustain
  parameters.add(std::make_unique<juce::AudioParameterFloat>(
      ParameterID::filterSustain, "Filter Sustain",
      juce::NormalisableRange<float>(0.0f, 100.0f, 1.0f), 0.0f,
      juce::AudioParameterFloatAttributes().withLabel("%")));

  // Filter Release
  parameters.add(std::make_unique<juce::AudioParameterFloat>(
      ParameterID::filterRelease, "Filter Release",
      juce::NormalisableRange<float>(0.0f, 100.0f, 1.0f), 25.0f,
      juce::AudioParameterFloatAttributes().withLabel("%")));

  // Envelope Attack
  parameters.add(std::make_unique<juce::AudioParameterFloat>(
      ParameterID::envAttack, "Env Attack",
      juce::NormalisableRange<float>(0.0f, 100.0f, 1.0f), 0.0f,
      juce::AudioParameterFloatAttributes().withLabel("%")));

  // Envelope Decay
  parameters.add(std::make_unique<juce::AudioParameterFloat>(
      ParameterID::envDecay, "Env Decay",
      juce::NormalisableRange<float>(0.0f, 100.0f, 1.0f), 50.0f,
      juce::AudioParameterFloatAttributes().withLabel("%")));

  // Envelope Sustain
  parameters.add(std::make_unique<juce::AudioParameterFloat>(
      ParameterID::envSustain, "Env Sustain",
      juce::NormalisableRange<float>(0.0f, 100.0f, 1.0f), 100.0f,
      juce::AudioParameterFloatAttributes().withLabel("%")));

  // Envelope Release
  parameters.add(std::make_unique<juce::AudioParameterFloat>(
      ParameterID::envRelease, "Env Release",
      juce::NormalisableRange<float>(0.0f, 100.0f, 1.0f), 30.0f,
      juce::AudioParameterFloatAttributes().withLabel("%")));

  // LFO Rate
  auto lfoRateStringFromValue = [](float value, int) {
    float lfoHz = std::exp(7.0f * value - 4.0f);
    return juce::String(lfoHz, 3);
  };
  parameters.add(std::make_unique<juce::AudioParameterFloat>(
      ParameterID::lfoRate, "LFO Rate", juce::NormalisableRange<float>(), 0.81f,
      juce::AudioParameterFloatAttributes()
          .withLabel("Hz")
          .withStringFromValueFunction(lfoRateStringFromValue)));

  // Vibrato
  auto vibratoStringFromValue = [](float value, int) {
    if (value < 0.0f)
      return "PWM " + juce::String(-value, 1);
    else
      return juce::String(value, 1);
  };
  parameters.add(std::make_unique<juce::AudioParameterFloat>(
      ParameterID::vibrato, "Vibrato",
      juce::NormalisableRange<float>(-100.0f, 100.0f, 0.1f), 0.0f,
      juce::AudioParameterFloatAttributes()
          .withLabel("%")
          .withStringFromValueFunction(vibratoStringFromValue)));

  // Noise
  parameters.add(std::make_unique<juce::AudioParameterFloat>(
      ParameterID::noise, "Noise",
      juce::NormalisableRange<float>(0.0f, 100.0f, 1.0f), 0.0f,
      juce::AudioParameterFloatAttributes().withLabel("%")));

  // Octave
  parameters.add(std::make_unique<juce::AudioParameterFloat>(
      ParameterID::octave, "Octave",
      juce::NormalisableRange<float>(-2.0f, 2.0f, 1.0f), 0.0f));

  // Tuning
  parameters.add(std::make_unique<juce::AudioParameterFloat>(
      ParameterID::tuning, "Tuning",
      juce::NormalisableRange<float>(-100.0f, 100.0f, 0.1f), 0.0f,
      juce::AudioParameterFloatAttributes().withLabel("cent")));

  // Output Level
  parameters.add(std::make_unique<juce::AudioParameterFloat>(
      ParameterID::outputLevel, "Output Level",
      juce::NormalisableRange<float>(-24.0f, 6.0f, 0.1f), 0.0f,
      juce::AudioParameterFloatAttributes().withLabel("dB")));

  return parameters;
}
