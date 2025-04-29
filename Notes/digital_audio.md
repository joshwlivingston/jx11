# Creating a Synthesizer with JUCE

## Chapter 3: Digital Audio

Digital audio is described by a computer as a stream of 44,100 or more numbers
per second. Each of these numbers is called a **sample**. The samples describe
how the speaker should move back and forth to vibrate the air and produce sound.

## Sampling Rate

When a digital signal is band-limited, the analog-to-digital converter (ADC) is
assumed to be able to represent the signal at a rate twice the signal's highest
frequency. Since human hearing tops out at roughly 20,000 Hz (lower for most
people), 40,000 Hz is a generally viable sampling rate for sound perceptible to
humans.

44,100 Hz (44.1 kHz) is used as a result of choices made in older technologies
like the CD, where the math just so happened to work out to 44.1 kHz. 48 kHz is
often used in practice; higher sampling rates _can_ be used, a technique known
as oversampling. However, oversampling is not the norm.

Lower sampling rates, such as 16 kHz and 8 kHz can be used; however these should
only be used in audio with few high frequencies, such as speech.

Given the sampling rate, the maximum frequency the signal can contain is known
as the **Nyquist limit**. If the highest frequency in the singal is below that
limit, the signal is said to be **band-limited** and the sampled signal is
_equivalent to the original_.

Interestingly, the signal itself can be _exactly_ reconstructed by convoluting
the sampled signal with the function `sin(pi * x) / (pi * x)`. Another funciton,
such as a polynomial or a linear equation, theoretically could be used to
describe the signal; however, there is only one _true_ mathematical solution. In
practice, an approximation to this solution is used, due to time required to
compute the actual true solution.

## Quantization and Bit Depth

While JUCE works with floats between 1 and -1, the ADC and DAC actually work
with integers, either 16 bit or 24 bit.

The ADC (the part in the microphone) rounds the voltage reading to the nearest
whole number, and uses that integer representation for the sample. This process
is known as **quantization**, and the introduced error is the
**quantization noise**.

The noise from the quantization error is 65,536 times quieter than the actual
sound in 16-bit sampling, a signal-to-noise ratio of 96dB. Additionally, DAW's
can add **dithering**, which is noise added to the MIDI to hide the
quantization noise.

Since humans
generally cannot hear sounds at -96 dB, 16-bit sampling is considered good
enough; however, 24-bit sampling is becoming increasingly common.

JUCE uses 32-bit single-precision floats, which have a precision of 24 bits. If
desired, you can also use 64-bit precision floats.

## Plugins

In plugins, digital audio works basically the same as through the ADC and DAC.
The only difference is that you create the sampled signal from scratch, and
send that synthesized signal to the DAC.

Witha sample rate of 44.1 kHz, the plugin needs to produce one 44,100 samples
per second, or one sample every 23 milliseconds. The DAC will convert this
sampled data into a continuous voltage used to send sound to the speakers.
