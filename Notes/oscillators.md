# Building Synthesizers with JUCE

## Chapter 5: Oscillators

## Sine Waves

All sounds can be broken down into component sine waves via fast fourier
transform. Because audio programming deals in samples, the sin formula is
modified to become:

```
y = amplitude x sin(2 * pi * t * frequency / sampleRate + phase)
```

where:

- `t`: the index of the sample
- `Frequency`: how often the sine wave repeats per seconds. A4 440 Hz means the
  note vibrates 440 times per second.
- `Period`: inverse of the _frequency_. Length of a single sine wave cycle
- `Amplitude`: Describes how high the sine wave goes. Important not to create
  waveforms with an amplitude over 1, or the sound will be "horribly messed
  up."
- `Phase`: The amount by which the sine wave is shifted on the time axis. If
  the phase is 90 degrees (pi / 2 radians), then it's a cosine wave. Phase is
  expressed in radians. "How much the sine wave is shifted in time."
