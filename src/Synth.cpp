/*
  ==============================================================================

    Synth.cpp
    Created: 5 May 2025 5:13:05pm
    Author:  Josh Livingston

  ==============================================================================
*/

#include "Synth.h"

#include "Utils.h"

Synth::Synth() { sampleRate = 44100.0f; }

void Synth::allocateResources(double sampleRate_, int /*samplePerBlock*/) {
  sampleRate = static_cast<float>(sampleRate_);
}

void Synth::deallocateResources() {
  // do nothing
}

void Synth::reset() {
  voice.reset();
  noiseGen.reset();
  pitchBend = 1.0f;
}

void Synth::render(float **outputBuffers, int sampleCount) {
  float *outputBufferLeft = outputBuffers[0];
  float *outputBufferRight = outputBuffers[1];

  voice.osc1.period = voice.period * pitchBend;
  voice.osc2.period = voice.osc1.period * detune;

  // loop through samples in buffer one by one
  for (int sample = 0; sample < sampleCount; ++sample) {
    // get next output from noise generator
    float noise = noiseGen.nextValue() * noiseMix;

    float outputLeft = 0.0f;
    float outputRight = 0.0f;

    // If key is pressed, calculate the new sample value
    float output = 0.0f;
    if (voice.env.isActive()) {
      output = voice.render(noise);
      outputLeft += output * voice.panLeft;
      outputRight += output * voice.panRight;
    }

    // Write the output value into audio buffer(s)
    outputBufferLeft[sample] = output;
    if (outputBufferRight != nullptr) {
      outputBufferRight[sample] = output;
    }
  }

  protectYourEars(outputBufferLeft, sampleCount);
  protectYourEars(outputBufferRight, sampleCount);
}

void Synth::midiMessage(uint8_t status, uint8_t data0, uint8_t data1) {
  switch (status & 0xF0) {
  // Note Off
  case 0x80: {
    noteOff(data0 & 0x7F);
    break;
  }

  // Note On
  case 0x90: {
    uint8_t note = data0 & 0x7f;
    uint8_t velo = data1 & 0x7f;
    if (velo > 0) {
      noteOn(note, velo);
    } else {
      noteOff(note);
    }
    break;
  }

  // Pitch bend
  case 0xE0:
    pitchBend = std::exp(-0.000014102f * float(data0 + 128 * data1 - 8192));
    break;
  }
}

void Synth::noteOn(int note, int velocity) {
  voice.note = note;

  float period = calcPeriod(note);
  voice.period = period;

  voice.osc1.amplitude = (velocity / 127.0f) * 0.5f;
  voice.osc2.amplitude = voice.osc1.amplitude * oscMix;

  /*
  When the resets are commented out, the phase does not shift.
  So, each repeated note is different. Result: more continuous wave-like.

  When not commented out, each repeated note will start the same -
  at the beginning of the phase

  voice.osc1.reset();
  voice.osc2.reset(); -
  */

  Envelope &env = voice.env;
  env.attackMultiplier = envAttack;
  env.decayMultiplier = envDecay;
  env.sustainLevel = envSustain;
  env.releaseMultiplier = envRelease;
  env.attack();
}

void Synth::noteOff(int note) {
  if (voice.note == note) {
    voice.release();
  }
}

float Synth::calcPeriod(int note) const {
  float period = tune * std::exp(-0.05776226505f * float(note));
  while (period < 6.0f || (period * detune) < 6.0f) {
    period += period;
  }
  return period;
}
