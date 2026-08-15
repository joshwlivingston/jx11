# Creating Synthesizer Plug-Ins with C++ and JUCE

Contains my work as I progress through Matthijs Hollemans'
[_Creating Synthesizer Plug-Ins with C++ and JUCE_](https://www.theaudioprogrammer.com/books/synth-plugin-book).
This project is creating a replica of the
[JX10](https://en.wikipedia.org/wiki/Roland_JX-10) synthesizer using the
[JUCE](https://juce.com/) framework.

The end result will be a synthesizer plugin, able to be used with any DAW.
Becuase this synth is a digital evolution of the JX10, Hollemans names this
synthesizer _JX11_.

## Build

Using Ninja:
```sh
cmake --preset ninja-debug
cmake --build --preset ninja-debug
```

Using Visual Studio 17 2022:
```sh
cmake --preset vs-debug
cmake --build --preset vs-debug
```

###  Windows

Note that on Windows, you may need to build from the "x64 Native Tools 
Command Prompt" to ensure the MSVC compiler is used.

