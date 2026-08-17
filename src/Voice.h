/*
  ==============================================================================

    NoiseGenerator.h
    Created: 5 May 2025 5:13:05pm
    Author:  Josh Livingston

  ==============================================================================
*/

#pragma once

#include "Envelope.h"
#include "Oscillator.h"

// The role of Voice is to produce the next output sample for a given note
struct Voice {
  int note;
  float saw;
  float period;
  float panLeft, panRight;
  Oscillator osc1;
  Oscillator osc2;
  Envelope env;
  float target;
  float glideRate;

  void reset() {
    note = 0;
    saw = 0;
    osc1.reset();
    osc2.reset();
    env.reset();
    panLeft = 0.707f;
    panRight = 0.707f;
  }

  void release() { env.release(); }

  float render(float input) {
    float sample1 = osc1.nextSample();
    float sample2 = osc2.nextSample();
    saw = saw * 0.997f + sample1 - sample2;

    float output = saw + input;

    float envelope = env.nextValue();
    return output * envelope;
  }

  /* Adjust left and right pan based on note
   * (low note = left, high note = right)
   */
  void updatePanning() {
    float panning = std::clamp((note - 60.0f) / 24.0f, -1.0f, 1.0f);
    panLeft = std::sin(PI_OVER_FOUR * (1.0f - panning));
    panRight = std::sin(PI_OVER_FOUR * (1.0f + panning));
  }

  void updateLFO() { period += glideRate * (target - period); }
};
