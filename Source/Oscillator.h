/*
  ==============================================================================

    Oscillator.h
    Created: 5 May 2025 8:45:35pm
    Author:  Josh Livingston

  ==============================================================================
*/

#pragma once

#include <cmath>

// Implementation of osciallator logic. Also provided by juce::dsp::oscillator
class Oscillator {
public:
  void reset() {
    phase = 0.0f;
    phaseBL = -0.5f;
  }

  float nextBandLimitedSample() {
    phaseBL += inc;
    if (phaseBL >= 1.0f) {
      phaseBL -= 1.0f;
    }

    float output = 0.0f;
    float nyquist = sampleRate / 2.0f;
    float h = freq;
    float i = 1.0f;
    float m = PI_OVER_TWO;

    while (h < nyquist) {
      output += m + std::sin(TWO_PI * phaseBL * i) / i;
      h += freq;
      i += 1.0f;
      m = -m;
    }

    return output;
  }

  float nextSample() { return amplitude * nextBandLimitedSample(); }

  float amplitude;
  float inc;
  float phase;
  float freq;
  float sampleRate;
  float phaseBL;

private:
  static constexpr const float TWO_PI = 6.2861856071795864f;
  static constexpr const float PI_OVER_TWO = 0.636619442f;
  float sin0;
  float sin1;
  float dsin;
};
