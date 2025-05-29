/*
  ==============================================================================

    NoiseGenerator.h
    Created: 5 May 2025 5:13:05pm
    Author:  Josh Livingston

  ==============================================================================
*/

#pragma once

#include "Oscillator.h"

// The role of Voice is to produce the next output sample for a given note
struct Voice {
  int note;
  float saw;
  Oscillator osc;

  void reset() {
    note = 0;
    saw = 0;
    osc.reset();
  }
  float render() {
    float sample = osc.nextSample();
    saw = saw * 0.997f + sample;
    return saw;
  }
};
