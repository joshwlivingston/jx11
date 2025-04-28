# Creating Synth Plugins with JUCE

## Chapter 3: MIDI In, Sound Out

MIDI devices communicate with each other by sending a short message consisting
of one, two, or three bytes.

1. The first byte is the **status** byte, indicating what command must be
   performed and on what channel. The status byte always has the most
   significant bit set to one, so its value is between 128 and 255, or 0x80 and
   0xFF in hexadecimal.

The possible commands are:

| Command | Description      |
| ------- | ---------------- |
| `8x`    | Note Off         |
| `9x`    | Note On          |
| `Ax`    | Key Pressure     |
| `Bx`    | Control Change   |
| `Cx`    | Program Change   |
| `Dx`    | Channel Pressure |
| `Ex`    | Pitch Bend       |
| `Fx`    | System Messages  |

Where `x` is the channel number and is within [0, 15]. The status byte may be
omitted if it is the same as the previous status byte, a process known as
**running status**. JUCE will always provide all three data bytes.

2. The second and third **data** bytes depend on the command used in the first
   byte. The most significant bit for the data bytes is always 0, to
   distinguish from the status byte. On rare occaision, the data bytes are
   combined into a 14-bit value.

All official MIDI specifications are available through the
[MIDI Association](https://midi.org/spec-detail)'s website, after creating a
free account.

## Note On and Note Off

In MIDI, notes do not have duration. Instead the **Note On** (**`9x`**) and
**Note Off** (**`8x`**) codes are used to determine timing of the notes played.
When a key is pressed, the Note On command is sent; when it is released, Note
Off is sent.

Example of a Note On message:

```
90 45 73
```

`90` is the status byte, indicating "Note On," `45` is the first data byte,
indicating the note to play, and `73` is the second data byte, indicating
the velocity. Velocity is also supported for "Note Off," can can be used on the
envelope, for example.

A "Note On" event with a velocity set to 0 is treated the same as a "Note Off"
event. So, using the running status protocol, you can send `90 XX YY` followed
by `XX 00` saving one byte, even though the command is "different."

A4: 440 Hz is data number 69, and the numbers increment and decrement by one,
indicating moving up or down one semitone.

## Control Change

The command **`Bx`** where `x` is the channel, indicates a **control change**,
applied to all notes currently being played in that channel. The first data
byte is the MIDI continuous controller (CC) to apply, and the second data byte
is the value of the control to apply.

Common MIDI CC codes:

| CC Code | Description                   |
| ------- | ----------------------------- |
| `01`    | Modulation Wheel              |
| `07`    | Channel Volume                |
| `0A`    | Panning                       |
| `0B`    | Expression                    |
| `40`    | Sustain Pedal                 |
| `7B`    | All Notes Off (panic / reset) |

The control change is the most powerful MIDI message; consequentally, it is the
most complicated.

## Pitch Bend

The **pitch bend** (**`Ex`**) is the _one_ MIDI controller has a message type of
its own. Similarly to a control change, the pitch bend is applied to all notes
being played in the selected channel.

The data bytes are combined into one 14-bit value for pitch bend, resulting in
an available range of -8192 to +8192.

## Aftertouch

The **key pressure** (**`Ax`**) and **channel pressure** (**`Dx`**) controls
change the character of the sound after the note has started playing and note is
pressed down further, an effect known as **aftertouch**. The channel pressure
has one data byte only.

## Program Change

The **program change** command (**`Cx`**) changes the current instrument that
is playing, a feature usually used with MIDI hardware. The program change has
one data byte only.

## System Messages

The **system messages** (**`Fx`**) are for things like timing, song position,
and other things generally not used in a digital synthesizer.

## Other Protocols

**MPE**, or **MIDI Polyphonic Expression** is a new protocol that allows MIDI
controls to be applied at the individual note level.

**MIDI 2.0** was released in 2020, marking significant improvements over the
original specification:

- Controller value precision now 32 bits, up from 7 bits
- 256 available channels, up from 16
- Per-note control, as in MPE
