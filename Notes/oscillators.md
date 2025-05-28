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
Many of those sine waves contain frequencies above the Nyquist limit; those
frequencies are mirrored to inharmonic lower frequencies - the aliases.

To prevent aliases, you can create a **bandlimited sawtooth**, which is a
sawtooth comprised of "wobbly" edges. "Bandlimited" refers to the feature of
these waves, that no sample frequency will exceed the sample's Nyquist limit.
These bands are achieved through applying the following equation:

```
(2 / pi) * (sin(phi) - sin(2 * phi) / 2 - sin(3 * phi) / 3 - ...)
```

In this case, `phi` represents the fundamental frequency. The second harmonic
therefore has frequency `2 * phi`; the third, `3 * phi`, and so on. `2 / pi` is
a scaling factor used to keep the values between -1 and 1.

This particular equation demonstrates a form of **additive synthesis**, a
technique in which sounds are created through _adding_ sine waves together.
While this technique is slow in practice, it is helpful for illustrative
purposes.

### Bandlimited Impulse Trains (BLIT)

An older way of making efficient sawtooth oscillators uses a technique known
as impulse trains. **Bandlimited impulse trains** are an older technique, and,
while more modern and better techniques now exist, _impulse trains_ are a core
concept in DSP.
