#pragma once

#include <juce_audio_processors/juce_audio_processors.h>
#include <juce_gui_basics/juce_gui_basics.h>

// An iOS 6 UISegmentedControl bound to a stepped parameter. Segment i maps to
// the normalised value i / (N - 1), which covers both choice parameters and
// stepped float parameters such as octave (-2...+2).
class SegmentedControl : public juce::Component {
public:
  SegmentedControl(juce::RangedAudioParameter &parameter,
                   juce::StringArray segmentLabels);
  ~SegmentedControl() override;

  void paint(juce::Graphics &) override;
  void mouseDown(const juce::MouseEvent &) override;

private:
  int segmentAt(float x) const;
  int indexForValue(float value) const;

  juce::RangedAudioParameter &parameter;
  const juce::StringArray labels;
  int selected = 0;
  juce::ParameterAttachment attachment;

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SegmentedControl)
};
