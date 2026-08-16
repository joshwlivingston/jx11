/*
  ==============================================================================

    Synth.h
    Created: 5 May 2025 5:13:05pm
    Author:  Josh Livingston

  ==============================================================================
*/

#pragma once

#include <stdint.h>

#include <juce_audio_basics/juce_audio_basics.h>
#include <juce_audio_processors/juce_audio_processors.h>
#include <juce_core/juce_core.h>
#include <juce_data_structures/juce_data_structures.h>
#include <juce_graphics/juce_graphics.h>
#include <juce_gui_basics/juce_gui_basics.h>

#include "NoiseGenerator.h"
#include "Voice.h"

class Synth {
public:
  Synth();

  void allocateResources(double sampleRate, int samplesPerBlock);
  void deallocateResources();
  void reset();
  void render(float **outputBuffers, int sampleCount);
  void midiMessage(uint8_t data0, uint8_t data1, uint8_t data2);
  float calcPeriod(int v, int note) const;

  float noiseMix;
  float envAttack;
  float envDecay;
  float envSustain;
  float envRelease;
  float oscMix;
  float detune;
  float tune;
  float pitchBend;
  float volumeTrim;
  float velocitySensitivity;
  bool ignoreVelocity;
  juce::LinearSmoothedValue<float> outputLevelSmoother;

  static constexpr int MAX_VOICES = 8;
  int numVoices;

private:
  void noteOn(int note, int velocity);
  void noteOff(int note);
  void startVoice(int v, int note, int velocity);
  int findFreeVoice() const;
  void restartMonoVoice(int note, int velocity);
  void shiftQueuedNotes();
  int nextQueuedNote();

  float sampleRate;
  std::array<Voice, MAX_VOICES> voices;
  NoiseGenerator noiseGen;
};
