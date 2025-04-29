# Creating a Synthesizer with JUCE

## Chapter 3: Digital Audio

Digital audio is described by a computer as a stream of 44,100 or more numbers
per second. Each of these numbers is called a **sample**. The samples describe
how the speaker should move back and forth to vibrate the air and produce sound.

## Sampling Rate

Under the assumption that the singal is band-limited, the analog-to-digital
converter (ADC) is assumed to be able to represent the digital signal at a rate
twice its highest frequency. Human hearing tops out at roughly 20,000 Hz (much
lower for most people). So, 40,000 Hz is the minimally viably sampling rate for
sound perceptible to humans.

Given the sampling rate, the maximum frequency the signal can contain is known
as the Nyquist limit. If the highest frequency in the singal is below that
limit, the sampled signal is _equivalent to the original_.

Additionally, given a sample of a signal, the signal itself can be _exactly_
reconstructed (by convoluting the sampled signal with the function
`sin(pi * x) / (pi * x)`. Another funciton, such as a polynomial or a linear
equation, theoretically could be used to describe the signal; however, there is
only one _true_ mathematical solution. In practice, an approximation to this
solution is used, due to time required to compute the actual true solution.

44,100 Hz (44.1 kHz) is used as a result of choices made in older technologies
like the CD, where the math just so happened to work out to 44.1 kHz. 48 kHz is
often used in practice; higher sampling rates _can_ be used, a technique known
as oversampling. However, oversampling is not the norm.

Lower sampling rates, such as 16 kHz and 8 kHz can be used; however these should
only be used in audio with few high frequencies, such as speech.

## Quantization and Bit Depth
