# Synthesizer Plugins with JUCE

## CHapter 4: Audio and MIDI Buffers

## Audio Buffer

The `juce::AudioBuffer` object is the audio block - the memory used to store the
samples. A typical block contains 128 samples.

A larger block size means less overhead is introduced via repeated calls to
`processBlock()`.

A smaller block size decreases the latency - the delay between asking for a
block and receiving it.

Choosing the smallest allowed buffer size, to minimize latency, seems like a
logical choice; however, smaller size requires faster processing time -
something limited by the computer, plugin, hardware, driver, etc. So, you have
to ensure that the buffer size chosen is able to be processed in that implied
latency time, a timing known as the "deadline."

To help meet the deadline, the `processBlock()` method runs on a high priority
thread, known as the audio thread. Many blocking operations, such as memory
allocation and system calls are forbidden on the audio thread. Additionally,
audio programming uses concepts such as atomic variables and lock-free circular
buffers, features not common in regular programming.

Latency is unavoidable in digital audio; ideally, the latency is so low that it
is unnoticeable.

Most DAW's allow the user to set the buffer size; "buffer" size is often used
interchangably with "block" size. In Logic Pro, for example, the buffer size
setting displays the resulting the latency based on the chosen buffer size.

### Automatic Block Size Updates in DAW

Developers need to be aware of the block size at any given time.

DAW's may update block sizes according to automation existing in the project. FL
Studio, for example, occaisonally calls `processBlock()` with a block size of 1,
which is not ideal.

Parameter changes are usually handled at the beginning of the block, so shorter
block sizes helps ensure meeting the deadline, even when automation is present.

## MIDI Buffer

The `juce::midiBuffer` object is an array of `midiMessage`'s, which can be
accessed via the
[`MidiBufferIterator`](https://docs.juce.com/master/classMidiBufferIterator.html).

The `midiMessage` includes the status byte, and the data byte(s).

It also includes timestamp at `.samplePosition`: the number of samples relative
to the start of audio buffer

The timestamp indicates when the command should occur. Since a block can span
multiple note on / note off cycles, for example, you need the timestamp to issue
the MIDI command at the correct time.

The first-pass strategy is to loop through the MIDI instructions and issue them;
however, that would lead to all instructions being issued at the beginning of
the block.

JX11 splits the Audio buffer into smaller pieces to deal with each MIDI command
as it appears. That way, all commands can be issued at either the beginning or
the end of the block.
