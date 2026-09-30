#include "SegmentedControl.h"
#include "Skin.h"
#include "Textures.h"

SegmentedControl::SegmentedControl(juce::RangedAudioParameter &param,
                                   juce::StringArray segmentLabels)
    : parameter(param), labels(std::move(segmentLabels)),
      attachment(param, [this](float value) {
        selected = indexForValue(value);
        repaint();
      }) {
  jassert(labels.size() > 1);
  attachment.sendInitialUpdate();
}

SegmentedControl::~SegmentedControl() {}

int SegmentedControl::indexForValue(float value) const {
  return juce::jlimit(0, labels.size() - 1,
                      juce::roundToInt(parameter.convertTo0to1(value) *
                                       float(labels.size() - 1)));
}

int SegmentedControl::segmentAt(float x) const {
  return juce::jlimit(0, labels.size() - 1,
                      int(x / float(getWidth()) * float(labels.size())));
}

void SegmentedControl::mouseDown(const juce::MouseEvent &e) {
  const int index = segmentAt(e.position.x);
  if (index == selected)
    return;

  attachment.setValueAsCompleteGesture(
      parameter.convertFrom0to1(float(index) / float(labels.size() - 1)));
}

void SegmentedControl::paint(juce::Graphics &g) {
  const auto bounds =
      getLocalBounds().toFloat().reduced(0.5f).withTrimmedBottom(1.0f);
  const float radius = 5.0f;
  const float segmentWidth = bounds.getWidth() / float(labels.size());

  juce::Path outline;
  outline.addRoundedRectangle(bounds, radius);

  // Inset into the surface: a lit lip along the bottom edge.
  g.setColour(juce::Colours::white.withAlpha(0.45f));
  g.fillRoundedRectangle(bounds.translated(0.0f, 1.0f), radius);

  {
    juce::Graphics::ScopedSaveState state(g);
    g.reduceClipRegion(outline);

    // Unselected segments: white, glossy, lit from above.
    g.setGradientFill({juce::Colour(0xfffdfdfd), 0.0f, bounds.getY(),
                       juce::Colour(0xffd2d4d7), 0.0f, bounds.getBottom(),
                       false});
    g.fillRect(bounds);
    Textures::drawGloss(g, bounds, 0.0f, 0.35f);

    // The selected segment is pressed in and blue.
    const auto chosen =
        bounds.withX(bounds.getX() + segmentWidth * float(selected))
            .withWidth(segmentWidth);
    juce::ColourGradient pressed(Skin::blueDark, 0.0f, chosen.getY(),
                                 Skin::blue, 0.0f, chosen.getBottom(), false);
    pressed.addColour(0.5, Skin::blue.darker(0.1f));
    g.setGradientFill(pressed);
    g.fillRect(chosen);

    juce::Path well;
    well.addRectangle(chosen);
    Textures::drawInnerShadow(g, well, juce::Colours::black.withAlpha(0.6f), 4,
                              {0, 2});

    // Hairline dividers with a highlight on their right-hand side.
    for (int i = 1; i < labels.size(); ++i) {
      const float x = bounds.getX() + segmentWidth * float(i);
      g.setColour(juce::Colours::black.withAlpha(0.3f));
      g.fillRect(x - 0.5f, bounds.getY(), 1.0f, bounds.getHeight());
      if (i != selected && i != selected + 1) {
        g.setColour(juce::Colours::white.withAlpha(0.6f));
        g.fillRect(x + 0.5f, bounds.getY(), 1.0f, bounds.getHeight());
      }
    }
  }

  g.setColour(juce::Colours::black.withAlpha(0.45f));
  g.strokePath(outline, juce::PathStrokeType(1.0f));

  const auto font = Skin::font(juce::jmin(13.0f, bounds.getHeight() * 0.48f));
  for (int i = 0; i < labels.size(); ++i) {
    const auto area = bounds.withX(bounds.getX() + segmentWidth * float(i))
                          .withWidth(segmentWidth);
    Textures::drawLetterpressText(
        g, labels[i].toUpperCase(), area, juce::Justification::centred,
        i == selected ? juce::Colours::white : juce::Colour(0xff4c5057), font);
  }
}
