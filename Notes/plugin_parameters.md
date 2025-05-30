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
