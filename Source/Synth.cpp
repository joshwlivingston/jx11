/*
  ==============================================================================

    NoiseGenerator.h
    Created: 5 May 2025 5:13:05pm
    Author:  Josh Livingston

  ==============================================================================
*/

#include "Synth.h"

#include "Utils.h"

Synth::Synth()
{
  sampleRate = 44100.0f;
}

void Synth::allocateResources(double sampleRate_, int /*samplePerBlock*/ )
{
  sampleRate = static_cast<float>(sampleRate_);
}

void Synth::deallocateResources()
{
  // do nothing
}

void Synth::reset()
{
    voice.reset();
    noiseGen.reset();
}

void Synth::render(float** outputBuffers, int sampleCount)
{
  float* outputBufferLeft = outputBuffers[0];
  float* outputBufferRight = outputBuffers[1];

  // loop through samples in buffer one by one
  for (int sample = 0; sample < sampleCount; ++sample) {
    // get next output from noise generator
    float noise  = noiseGen.nextValue();

    // If key is pressed, calculate the new sample value
    float output = 0.0f;
    if (voice.note > 0) {
      // noise value is multipled by the normalized velocity
      // cut in half for realistic gain
        output = noise * (voice.velocity / 127.0f) * 0.5f;
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

void Synth::midiMessage(uint8_t status, uint8_t data0, uint8_t data1)
{
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
  }
}

void Synth::noteOn(int note, int velocity)
{
  voice.note = note;
  voice.velocity = velocity;
}

void Synth::noteOff(int note)
{
  if (voice.note == note) {
    voice.note = 0;
    voice.velocity = 0;
  }
}
