/*
  ==============================================================================

    NoiseGenerator.h
    Created: 5 May 2025 5:13:05pm
    Author:  Josh Livingston

  ==============================================================================
*/

#pragma once

#include "Envelope.h"
#include "Filter.h"
#include "Oscillator.h"

#include <algorithm>
#include <cmath>

// The role of Voice is to produce the next output sample for a given note
struct Voice {
  int note;
  float saw;
  float period;
  float panLeft, panRight;
  Oscillator osc1;
  Oscillator osc2;
  Envelope env;
  Filter filter;
  Envelope filterEnv;
  float target;
  float glideRate;
  float cutoff;
  float filterMod;
  float filterQ;
  float pitchBend;
  float filterEnvDepth;

  void reset() {
    note = 0;
    saw = 0;
    osc1.reset();
    osc2.reset();
    env.reset();
    filter.reset();
    filterEnv.reset();
    panLeft = 0.707f;
    panRight = 0.707f;
  }

  void release() {
    env.release();
    filterEnv.release();
  }

  float render(float input) {
    float sample1 = osc1.nextSample();
    float sample2 = osc2.nextSample();
    saw = saw * 0.997f + sample1 - sample2;

    float output = saw + input;

    output = filter.render(output);

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

  void updateLFO() {
    period += glideRate * (target - period);
    float fenv = filterEnv.nextValue();
    float modulatedCutoff =
        cutoff * std::exp(filterMod + filterEnvDepth * fenv) / pitchBend;
    modulatedCutoff = std::clamp(modulatedCutoff, 30.0f, 20000.0f);
    filter.updateCoefficients(modulatedCutoff, filterQ);
  }
};
