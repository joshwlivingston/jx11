/*
  ==============================================================================

    Synth.h
    Created: 5 May 2025 5:13:05pm
    Author:  Josh Livingston

  ==============================================================================
*/

#pragma once

#include <stdint.h>

#include <JuceHeader.h>

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

private:
  void noteOn(int note, int velocity);
  void noteOff(int note);

  float sampleRate;
  Voice voice;
  NoiseGenerator noiseGen;
};
