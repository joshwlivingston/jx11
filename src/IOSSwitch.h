#pragma once

#include <juce_gui_basics/juce_gui_basics.h>

// An iOS 5 UISwitch. It's a ToggleButton underneath, so a regular
// ButtonAttachment drives it; the thumb animates to each new state.
class IOSSwitch : public juce::ToggleButton {
public:
  IOSSwitch(juce::String onText, juce::String offText);
  ~IOSSwitch() override;

  void paintButton(juce::Graphics &g, bool shouldDrawButtonAsHighlighted,
                   bool shouldDrawButtonAsDown) override;

private:
  void advance(double timestampSec);

  const juce::String onText, offText;
  float position = 0.0f; // 0 = off, 1 = on
  double lastTimestamp = 0.0;
  juce::VBlankAttachment vblank;

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(IOSSwitch)
};
