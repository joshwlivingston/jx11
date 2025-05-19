/*
  ==============================================================================

    Oscillator.h
    Created: 5 May 2025 8:45:35pm
    Author:  Josh Livingston

  ==============================================================================
*/

#pragma once

// Implementation of osciallator logic. Also provided by juce::dsp::oscillator
class Oscillator {
public:
  void reset() { 
    phase = 0.0f;
  }

  float nextSample()
  {
    phase += inc;
    if (phase >= 1.0f) {
      phase -= 1.0f;
    }
    return amplitude * (2.0f * phase - 1.0f);
  }

  float amplitude;
  float inc;
  float phase;

private:
  static constexpr const float TWO_PI = 6.2861856071795864f;
  float sin0;
  float sin1;
  float dsin;
};
