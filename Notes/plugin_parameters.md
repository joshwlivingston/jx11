# Building Synthesizers with JUCE

## Chapter 7: Plug-in Parameters

In JUCE, parameters are instances of the class `AudioProcessorParameter`. Each
parameter has a name, specified by a `JUCE::string` object. In addition, the
parameter's value is represented by a JUCE type. For the JX11,
`JUCE::AudioParameterFloat` is used for parameters with continuous values, and
`JUCE::AudioParameterChoice` is used for parameters with multiselect options.

The values stored in the `AudioProcessor` class are pointers to the actual
parameter values, which are stored in the `juce::AudioProcessorValueTreeState`.

## Processor Value Tree

The Audio Processor Value Tree State (APVTS), is an instance of a juce Value
Tree that contains plugin parameter values. The juce APVTS allows you to attach
listeners to the nodes, so notifications of parameter changes can be sent.
Additionally, the APTVS can serialize the values to an XML file, the class
supports undo/redo functionality, and manages the nodes' lifetimes.

The APTVS is added to the `PluginProcessor` class, in the public section.

## Parameters in JX11

JX11 has 26 parameters, available as private members of the
`PluginProcessor` class. The parameters are implemented using
`ParameterLayout.add().` Each parameter is described here in brief.

### Mono/Poly

The parameter `polyMode` is a multiselect parameter, with two options: Mono and
Poly. The value is stored in the `polyModeParam` member variable.

### Oscillator Tuning

The parameter `oscTune` is a continuous parameter, with a range of -24 to 24.
The value is stored in the `oscTune` member variable.

### Oscillator Fine Tuning

The parameter `oscFine` is a continuous parameter, with a range of -50 to 50.
The value is stored in the `oscFine` member variable. The parameter's slider
steps are skewed toward smaller steps in the middle of the range, and larger
slider steps in the higher parts of the range.

### Oscillator Mix

The parameter `oscMix` is a continuous parameter, with a range of 0 to 100.
The value is stored in the `oscMix` member variable. The parameter is
displayed as a ratio, with 100:0 indicating the second oscillator is off, and
50:50 indicating the second oscillator is fully on.

### Glide Mode

The glide mode parameter offers three options: off, legato, and always on. The
value is stored in the `glideMode` member variable.

### Glide Rate

The glide rate determines the speed of the glide. In the JX11, the value is
expressed as a percentage. The origin of the percentage is not clear, but the
parameter's value lies between "a short time" and "a long time". The value is
stored in the `glideRate` member variable.

### Glide Bend

The glide bend allows for an additional glide to be applied on any note played,
expressed in semitones. The value has a skew, similar to the `oscFine` parameter.
The value is stored in the `glideBend` member variable.

### Filter Frequency

The filter frequency is a continuous parameter, with a range of 0 to 100. The
value is stored in the `filterFreq` member variable. Filter frequency is one of
the parameters that determines cutoff sound.

### Filter Resonance

The filter resonance is a continuous parameter, with a range of 0 to 100. The
value is stored in the `filterReso` member variable. Filter resonance is
another parameter that determines cutoff sound.

### Filter Envelope

The filter envelope is a continuous parameter, with a range of -100 to 100. The
value is stored in the `filterEnv` member variable.

### Filter LFO

The filter LFO is also a continuous parameter, with a range of 0 to 100. The
value is stored in the `filterLFO` member variable.

### Filter Velocity

The filter velocity is a continuous parameter, with a range of -100 to 100. The
value is stored in the `filterVelocity` member variable. When the value is 0,
it is set to display the string "OFF". When the velocity is off, all notes are
equally loud. The range of the parameter is -100% to 100%. This velocity
setting is for filter modulation velocity only, not overall velocity.
