/*
  ==============================================================================

    NoiseGenerator.h
    Created: 5 May 2025 5:13:05pm
    Author:  Josh Livingston

  ==============================================================================
*/

#pragma once

#include "Oscillator.h"
#include "Envelope.h"

// The role of Voice is to produce the next output sample for a given note
struct Voice {
  int note;
  float saw;
  float period;
  Oscillator osc1;
  Oscillator osc2;
  Envelope env;

  void reset() {
    note = 0;
    saw = 0;
    osc1.reset();
    osc2.reset();
    env.reset();
  }

  void release()
  {
      env.release();
  }

  float render(float input) {
    float sample1 = osc1.nextSample();
    float sample2 = osc2.nextSample();
    saw = saw * 0.997f + sample1 - sample2;

    float output = saw + input;

    float envelope = env.nextValue();
    return output * envelope;
  }
};
