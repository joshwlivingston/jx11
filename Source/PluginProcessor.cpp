/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
JX11AudioProcessor::JX11AudioProcessor()
  #ifndef JucePlugin_PreferredChannelConfigurations
    : AudioProcessor(BusesProperties()
      #if !JucePlugin_IsMidiEffect
        #if !JucePlugin_IsSynth
          .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
        #endif
          // creates a BusesProperties() object with stereo output bus named
          // "Output"
          .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
      #endif)
  #endif // if blocks correspond to settings in Projucer
{
}

JX11AudioProcessor::~JX11AudioProcessor()
{
}

//==============================================================================
const juce::String JX11AudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool JX11AudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool JX11AudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool JX11AudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double JX11AudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int JX11AudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int JX11AudioProcessor::getCurrentProgram()
{
    return 0;
}

void JX11AudioProcessor::setCurrentProgram (int index)
{
}

const juce::String JX11AudioProcessor::getProgramName (int index)
{
    return {};
}

void JX11AudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================

// prepareToPlay() is called before the host start using the plugin; it
// communicates the current sample rate and the maximum block size to expect.
//
// DAW's will update block sizes according to any automation existing in the
// project. FL Studio, for example, occaisonally calls processBlock() with a
// block size of 1, which is not ideal.
//
// DAW's will update block sizes according to any automation existing in the
// project. Parameter changes are usually handled at the beginning of the block,
// so shorter block sizes helps ensure meeting the deadline, even when
// automation is present. Developers need to be aware of the DAW's chosen block
// size.
void JX11AudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
}

void JX11AudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool JX11AudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
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

// Audio callback method. Called once per block. A typical block contains 128
// samples. 
//
// A larger block size means less overhead is introduced via repeated calls to
// processBlock().
//
// A smaller block size decreases the latency - the delay between asking for a
// block and receiving it.
//
// Choosing the smallest allowed buffer size, to minimize latency, seems like a
// logical choice; however, smaller size requires faster processing time -
// something limited by the computer, plugin, hardware, driver, etc. So, you
// have to ensure that the buffer size chosen is able to be processed in that
// implied latency time, a timing known as the "deadline."
//
// To help meet the deadline, the processBlock() method runs on a high priority
// thread, known as the audio thread. Many blocking operations, such as memory
// allocation and system calls are forbidden on the audio thread. Additionally,
// audio programming uses concepts such as atomic variables and lock-free
// circular buffers, features not common in regular programming.
//
// Latency is unavoidable in digital audio; ideally, the latency is so low that
// it is unnoticeable.
//
// Most DAW's allow the user to set the buffer size; "buffer" size is often used
// interchangably with "block" size. In Logic Pro, for example, the buffer size
// setting displays the resulting the latency based on the chosen buffer size.
void JX11AudioProcessor::processBlock (
  juce::AudioBuffer<float>& buffer, // Where to place outgoing MIDI messages
  juce::MidiBuffer& midiMessages)   // Contains incoming MIDI messages
{
  // Sets a CPU flag that automatically truncates small floats to 0, instead of
  // converting them to denormals, which are much slower than regular floats
  juce::ScopedNoDenormals noDenormals;

  const auto totalNumInputChannels  = getTotalNumInputChannels();
  const auto totalNumOutputChannels = getTotalNumOutputChannels();

  // JX11 does not have an audio input channel, and JUCE does not guarantee that
  // the AudioBuffer channel will be empty. So, it's good defensive practice to
  // ensure the incoming audio buffer contains only silence.
  for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i) {
    buffer.clear(i, 0, buffer.getNumSamples());
  }

  // To look at the incoming MIDI messages, iterate through the buffer
  // In the `for` range, type MidiBufferIterator returns MidiMessageMetadata
  // https://docs.juce.com/master/classMidiBufferIterator.html
  for (const auto metadata : midiMessages) {
    if (metadata.numBytes == 3) {
      // 1 status byte (metadata[0])
      // 2 data bytes (metadata[1] and metadata[2])
      // 
      // also includes timestamp at metadata.samplePosition: the number of
      // samples relative to the start of audio buffer
      //
      // The timestamp indicates when the command should occur; since a block
      // can space mutliple note on/ note odd cycles, for example, you need the
      // timestamp to issue the MIDI command at the correct time
      //
      // The first-pass strategy is to loop through the MIDI instructions and
      // issue them; however, that would lead to all instructions being issued
      // at the beginning of the block.
      //
      // JX11 splits the Audtio buffer into smaller pieces to deal with each command
    }
  }

  // generate output audio for playing the notes
  for (int sample = 0; sample < buffer.getNumSamples(); ++sample) {
    // output audio...
  }
}

void JX11AudioProcessor::splitBufferByEvents(juce::AudioBuffer<float>& buffer,
                                             juce::MidiBudder& midiMessages)
{
  int bufferOffest = 0;
  for (const auto metadata : midiMessages) {
    const int samplesThisSegment = metadata.samplePosition - bufferOffset;
    if (samplesThisSegment > 0) {
      render(buffer, samplesThisSegment, bufferOffset);
      bufferOffset += samplesThisSegment;
    }

    if (metadata.numBytes <= 3) {
      uint8_t data1 = metadata.numBytes >= 2 ? metadata.data[1] : 0;
      uint8_t data2 = metadata.numBytes == 3 ? metadata.data[2] : 0;
      handleMIDI(metadata.data[0], data1, data2);
    }
  }

  int samplesLastSegment = buffer.getNumSamples() - bufferOffest;
  if (samplesLastSegment > 0) {
    render(buffer, samplesLastSegment, bufferOffest);
  }

  midiMessages.clear();
}

//==============================================================================
bool JX11AudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* JX11AudioProcessor::createEditor()
{
    return new JX11AudioProcessorEditor (*this);
}

//==============================================================================
void JX11AudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void JX11AudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new JX11AudioProcessor();
}
