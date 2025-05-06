/*
  ==============================================================================

    NoiseGenerator.h
    Created: 5 May 2025 5:13:05pm
    Author:  Josh Livingston

  ==============================================================================
*/

#pragma once

// The role of Voice is to produce the next output sample for a given note
struct Voice
{
  int note;
  int velocity;

  void reset()
  {
    note = 0;
    velocity = 0;
  }
};
