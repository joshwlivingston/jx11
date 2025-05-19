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

## Sawtooth Waves

**Sawtooth waves** are formed taking the phase value, which is the position of
the sample in the wave. In JUCE, since the samples are between -1 and 1, we take
`2 * phase - 1` for the _naive sawtooth_.

### Bandlimited Sawtooth

The naive sawtooth produces **aliases**, which are inharmonics resulting from the
sine waves used to create sharp corners like those found in the naive sawtooth.
Many of those sine waves contain frequences above the Nyquist limit; those
frequencies are mirrored to inharmonic lower frequencies - the aliases.

To prevent aliases, you can create a **bandlimited sawtooth**, which is a
sawtooth comprised of "wobbly" edges.
