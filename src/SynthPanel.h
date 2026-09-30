#pragma once

#include <juce_gui_basics/juce_gui_basics.h>
#include <vector>

#include "Textures.h"

// The instrument's body: walnut end cheeks, a stitched leather header and a
// brushed aluminium faceplate. It's always Skin::baseWidth x baseHeight; the
// editor scales it as a whole, and every control lives inside it.
class SynthPanel : public juce::Component {
public:
  SynthPanel();
  ~SynthPanel() override;

  void paint(juce::Graphics &) override;

  // Prints a caption above a child control that has no label of its own.
  void addCaption(juce::Component &target, const juce::String &text);

private:
  void paintFaceplate(juce::Graphics &g, juce::Rectangle<float> area);
  void paintHeader(juce::Graphics &g, juce::Rectangle<float> area);
  void paintCheek(juce::Graphics &g, juce::Rectangle<float> area,
                  Textures::CachedImage &wood, bool isLeft);
  void paintLogo(juce::Graphics &g, juce::Rectangle<float> area);

  struct Caption {
    juce::Component *target;
    juce::String text;
  };
  std::vector<Caption> captions;

  Textures::CachedImage metal, hide, leftWood, rightWood;

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SynthPanel)
};
