cmake_minimum_required(VERSION 3.22)
project(JX11 VERSION 1.0.0)

# Generate compile_commands.json, used by clangd
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Point this to wherever your JUCE framework is located. 
set(JUCE_ROOT "$ENV{HOME}/app/JUCE")
add_subdirectory("${JUCE_ROOT}" build_juce)

# Define the plugin and its properties
juce_add_plugin(JX11
    COMPANY_NAME "SquareRock"
    PLUGIN_MANUFACTURER_CODE "SQRK" 
    PLUGIN_CODE "Jx11"              
    FORMATS AU Standalone         
    PRODUCT_NAME "JX11"
    IS_SYNTH TRUE
    NEEDS_MIDI_INPUT TRUE
    NEEDS_MIDI_OUTPUT TRUE
    IS_MIDI_EFFECT FALSE
    EDITOR_WANTS_KEYBOARD_FOCUS TRUE
    COPY_PLUGIN_AFTER_BUILD TRUE
)

# Add compile options
target_compile_definitions(JX11 PRIVATE
    JUCE_STRICT_REFCOUNTEDPOINTER=1
)

# Add source code and headers
target_sources(JX11 PRIVATE
    src/Envelope.h
    src/NoiseGenerator.h
    src/Oscillator.h
    src/PluginEditor.cpp
    src/PluginEditor.h
    src/PluginProcessor.cpp
    src/PluginProcessor.h
    src/Preset.h
    src/Synth.cpp
    src/Synth.h
    src/Utils.h
    src/Voice.h
)

# Link necessary JUCE modules
target_link_libraries(JX11 PRIVATE
    juce::juce_audio_basics
    juce::juce_audio_devices
    juce::juce_audio_formats
    juce::juce_audio_plugin_client
    juce::juce_audio_processors
    juce::juce_audio_processors_headless
    juce::juce_audio_utils
    juce::juce_core
    juce::juce_data_structures
    juce::juce_events
    juce::juce_graphics
    juce::juce_gui_basics
    juce::juce_gui_extra
)

if(APPLE)
    set(USER_COMPONENT_DIR "$ENV{HOME}/Library/Audio/Plug-Ins/Components")
    add_custom_command(
        TARGET JX11_AU POST_BUILD
        COMMAND codesign --force --deep --sign - "${USER_COMPONENT_DIR}/JX11.component"
    )
endif()
