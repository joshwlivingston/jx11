# Building Synthesizers with JUCE

## Chapter 7: Plug-in Parameters

In JUCE, parameters are instances of the class `AudioProcessorParameter`. Each
parameter has a name, specified by a `JUCE::string` object. In addition, the
parameter's value is represented by a JUCE type. For the JX11,
`JUCE::AudioParameterFloat` is used for parameters with continuous values, and
`JUCE::AudioParameterChoice` is used for parameters with multiselect options.

The values stored in the `AudioProcessor` class are pointers to the actual
parameter values, which are stored in the `juce::AudioProcessorValueTreeState`.
