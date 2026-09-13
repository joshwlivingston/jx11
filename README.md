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

First, ensure [cmake](https://cmake.org/) is installed:
```sh
cmake --version
```

Then, build using one of the pre-defined presets:

### Default / Ninja
```sh
cmake --preset debug
cmake --build --preset debug
```

### Visual Studio 17 2022
```sh
cmake --preset vs2022-debug
cmake --build --preset vs2022-debug
```

To use a custom `AudioPluginHost` executable when debugging on Windows, configure with `-DAUDIO_PLUGIN_HOST_PATH`:
```sh
cmake --preset vs2022-debug -DAUDIO_PLUGIN_HOST_PATH="C:/path/to/AudioPluginHost.exe"
```

##  Windows

Note that on Windows, you may need to use the "x64 Native Tools 
Command Prompt" to ensure the MSVC compiler is used.
