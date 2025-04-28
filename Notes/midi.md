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

Where `x` is the channel number and is within [0, 15].

2. The second and third **data** bytes depend on the command used in the first
   byte. The most significant bit for the data bytes is always 0, to
   distinguish from the status byte. On rare occaision, the data bytes are
   combined into a 14-bit value.

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

A4: 440 Hz is data number 69, and the numbers increment and decrement by one,
indicating moving up or down one semitone.
