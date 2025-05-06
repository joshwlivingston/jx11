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
    phase = 0;
    sin0 = amplitude * std::sin(phase * TWO_PI);
    sin1 = amplitude * std::sin((phase - inc) * TWO_PI);
    dsin = 2.0f * std::cos(inc * TWO_PI);
  }

  float nextSample()
  {
    float sinx = dsin * sin0 - sin1;
    sin1 = sin0;
    sin0 = sinx;
    return sinx;
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
