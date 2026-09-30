#pragma once

#include <juce_audio_processors/juce_audio_processors.h>
#include <juce_gui_basics/juce_gui_basics.h>

// A backlit, iTunes-style LCD showing the current program, flanked by chrome
// step buttons. Clicking the glass opens the full preset list.
class PresetDisplay : public juce::Component, private juce::Timer {
public:
  explicit PresetDisplay(juce::AudioProcessor &processor);
  ~PresetDisplay() override;

  void paint(juce::Graphics &) override;
  void resized() override;
  void mouseUp(const juce::MouseEvent &) override;

private:
  class StepButton : public juce::Button {
  public:
    explicit StepButton(bool pointsLeft);
    void paintButton(juce::Graphics &g, bool shouldDrawButtonAsHighlighted,
                     bool shouldDrawButtonAsDown) override;

  private:
    const bool pointsLeft;
  };

  void timerCallback() override;
  void selectProgram(int index);
  void step(int delta);
  void showPresetMenu();

  juce::AudioProcessor &processor;
  StepButton previousButton{true}, nextButton{false};
  juce::Rectangle<int> screen;
  int shownProgram = -1;

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PresetDisplay)
};
