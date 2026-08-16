/*
  ==============================================================================

    Synth.cpp
    Created: 5 May 2025 5:13:05pm
    Author:  Josh Livingston

  ==============================================================================
*/

#include "Synth.h"

#include "Utils.h"

static const float ANALOG = 0.002f;

Synth::Synth() { sampleRate = 44100.0f; }

void Synth::allocateResources(double sampleRate_, int /*samplePerBlock*/) {
  sampleRate = static_cast<float>(sampleRate_);
}

void Synth::deallocateResources() {
  // do nothing
}

void Synth::reset() {
  for (int v = 0; v < MAX_VOICES; ++v) {
    voices[v].reset();
  }
  noiseGen.reset();
  pitchBend = 1.0f;
  outputLevelSmoother.reset(sampleRate, 0.05);
  lfo = 0.0f;
  lfoStep = 0;
  modWheel = 0.0f;
}

void Synth::render(float **outputBuffers, int sampleCount) {
  float *outputBufferLeft = outputBuffers[0];
  float *outputBufferRight = outputBuffers[1];

  for (int v = 0; v < MAX_VOICES; ++v) {
    Voice &voice = voices[v];
    if (voice.env.isActive()) {
      voice.osc1.period = voice.period * pitchBend;
      voice.osc2.period = voice.osc1.period * detune;
    }
  }

  // loop through samples in buffer one by one
  for (int sample = 0; sample < sampleCount; ++sample) {
    // LFO should be updated before samples are recalculated
    updateLFO();

    // get next output from noise generator
    float noise = noiseGen.nextValue() * noiseMix;

    float outputLeft = 0.0f;
    float outputRight = 0.0f;

    // If key is pressed, calculate the new sample value
    for (int v = 0; v < MAX_VOICES; ++v) {
      Voice &voice = voices[v];
      if (voice.env.isActive()) {
        float output = voice.render(noise);
        outputLeft += output * voice.panLeft;
        outputRight += output * voice.panRight;
      }
    }
    float outputLevel = outputLevelSmoother.getNextValue();
    outputLeft *= outputLevel;
    outputRight *= outputLevel;

    // Write the output value into audio buffer(s)
    if (outputBufferRight != nullptr) {
      outputBufferRight[sample] = outputLeft;
      outputBufferLeft[sample] = outputRight;
    } else {
      outputBufferLeft[sample] = (outputLeft + outputRight) * 0.5f;
    }
  }

  for (int v = 0; v < MAX_VOICES; ++v) {
    Voice &voice = voices[v];
    if (!voice.env.isActive()) {
      voice.env.reset();
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
  case 0xE0: {
    // bend up or down by two semitones
    static const float TWO_SEMITONES = -0.000014102f;
    pitchBend = std::exp(TWO_SEMITONES * float(data0 + 128 * data1 - 8192));
    break;
  }

  // Mod wheel
  case 0xB0: {
    uint8_t controller = data0 & 0x7F;
    uint8_t value = data1 & 0x7F;
    if (controller == 0x01) { // Mod Wheel
      modWheel = 0.000005f * float(value * value);
    }
    break;
  }
  }
}

void Synth::noteOn(int note, int velocity) {
  if (ignoreVelocity) {
    velocity = 80;
  }

  int v = 0;
  if (numVoices == 1) {
    // monophonic
    if (voices[0].note > 0) {
      // legato-style playing
      shiftQueuedNotes();
      restartMonoVoice(note, velocity);
      return;
    }
  } else {
    // polyphonic
    v = findFreeVoice();
  }
  startVoice(v, note, velocity);
}

void Synth::startVoice(int v, int note, int velocity) {
  float period = calcPeriod(v, note);

  Voice &voice = voices[v];
  voice.period = period;
  voice.note = note;

  // Uncomment here to apply pitch-based panning
  // voice.updatePanning();

  float vel = 0.004f * float((velocity + 64) * (velocity + 64)) - 8.0f;
  voice.osc1.amplitude = volumeTrim * vel;
  voice.osc2.amplitude = voice.osc1.amplitude * oscMix;

  if (vibrato == 0.0f && pwmDepth > 0.0f) {
    voice.osc2.squareWave(voice.osc1, voice.period);
  }

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
  if ((numVoices == 1) && (voices[0].note == note)) {
    int queuedNote = nextQueuedNote();
    if (queuedNote > 0) {
      restartMonoVoice(queuedNote, -1);
    }
  }
  for (int v = 0; v < MAX_VOICES; v++) {
    Voice &voice = voices[v];
    if (voice.note == note) {
      voice.release();
      voice.note = 0;
    }
  }
}

float Synth::calcPeriod(int v, int note) const {
  float period =
      tune * std::exp(-0.05776226505f * (float(note) + ANALOG * float(v)));
  while (period < 6.0f || (period * detune) < 6.0f) {
    period += period;
  }
  return period;
}

int Synth::findFreeVoice() const {
  int v = 0;
  float l = 100.0f;

  for (int i = 0; i < MAX_VOICES; ++i) {
    if (voices[i].env.level < l && !voices[i].env.isInAttack()) {
      l = voices[i].env.level;
      v = i;
    }
  }
  return v;
}

void Synth::restartMonoVoice(int note, int velocity) {
  float period = calcPeriod(0, note);
  Voice &voice = voices[0];
  voice.period = period;

  voice.env.level += SILENCE + SILENCE;
  voice.note = note;

  // Uncomment here to include pitch-based panning
  // voice.updatePanning();
}

void Synth::shiftQueuedNotes() {
  for (int tmp = MAX_VOICES - 1; tmp > 0; tmp--) {
    voices[tmp].note = voices[tmp - 1].note;
    voices[tmp].release();
  }
}

int Synth::nextQueuedNote() {
  int held = 0;
  for (int v = MAX_VOICES - 1; v > 0; v--) {
    if (voices[v].note > 0) {
      held = v;
    }
  }
  if (held > 0) {
    int note = voices[held].note;
    voices[held].note = 0;
    return note;
  }
  return 0;
}

void Synth::updateLFO() {
  // Decrement lfoStep on each function call
  if (--lfoStep <= 0) {
    // Enter once per LFO_MAX steps

    // Reset lfo stepper to max
    lfoStep = LFO_MAX;

    // Lfo steps between -pi and pi
    lfo += lfoInc;
    if (lfo > PI) {
      lfo -= TWO_PI;
    }

    // Calculate sine value
    const float sine = std::sin(lfo);

    // Calculate a vibrato amount and assign to oscillators
    float vibratoMod = 1.0f + sine * (modWheel + vibrato);
    float pwm = 1.0f + sine * (modWheel + pwmDepth);
    for (int v = 0; v < MAX_VOICES; ++v) {
      Voice &voice = voices[v];
      if (voice.env.isActive()) {
        voice.osc1.modulation = vibratoMod;
        voice.osc2.modulation = pwm;
      }
    }
  }
}
