#pragma once

#include <juce_graphics/juce_graphics.h>
#include <juce_gui_basics/juce_gui_basics.h>
#include <map>
#include <utility>

class LookAndFeel : public juce::LookAndFeel_V4 {
public:
  LookAndFeel();

  void drawRotarySlider(juce::Graphics &g, int x, int y, int width, int height,
                        float sliderPos, float rotaryStartAngle,
                        float rotaryEndAngle, juce::Slider &slider) override;

  void drawLinearSlider(juce::Graphics &g, int x, int y, int width, int height,
                        float sliderPos, float minSliderPos, float maxSliderPos,
                        const juce::Slider::SliderStyle style,
                        juce::Slider &slider) override;

  int getSliderThumbRadius(juce::Slider &slider) override;
  juce::Slider::SliderLayout getSliderLayout(juce::Slider &slider) override;
  juce::Label *createSliderTextBox(juce::Slider &slider) override;

  void drawLabel(juce::Graphics &g, juce::Label &label) override;
  void fillTextEditorBackground(juce::Graphics &g, int width, int height,
                                juce::TextEditor &editor) override;
  void drawTextEditorOutline(juce::Graphics &g, int width, int height,
                             juce::TextEditor &editor) override;

  void drawGroupComponentOutline(juce::Graphics &g, int w, int h,
                                 const juce::String &text,
                                 const juce::Justification &position,
                                 juce::GroupComponent &group) override;

  void drawPopupMenuBackground(juce::Graphics &g, int width,
                               int height) override;
  void drawPopupMenuItem(juce::Graphics &g, const juce::Rectangle<int> &area,
                         bool isSeparator, bool isActive, bool isHighlighted,
                         bool isTicked, bool hasSubMenu,
                         const juce::String &text,
                         const juce::String &shortcutKeyText,
                         const juce::Drawable *icon,
                         const juce::Colour *textColour) override;
  juce::Font getPopupMenuFont() override;
  void getIdealPopupMenuItemSize(const juce::String &text, bool isSeparator,
                                 int standardMenuItemHeight, int &idealWidth,
                                 int &idealHeight) override;

private:
  static constexpr float faderThumbDiameter = 26.0f;

  // Knob bodies are expensive to draw, so they're rendered once per
  // (logical size, physical size) and reused by every knob of that size.
  std::map<std::pair<int, int>, juce::Image> knobCache;

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(LookAndFeel);
};
