#include "IOSSwitch.h"
#include "Skin.h"
#include "Textures.h"
#include <cmath>

IOSSwitch::IOSSwitch(juce::String on, juce::String off)
    : onText(std::move(on)), offText(std::move(off)),
      vblank(this, [this](double timestampSec) { advance(timestampSec); }) {
  setClickingTogglesState(true);
  setMouseCursor(juce::MouseCursor::PointingHandCursor);
}

IOSSwitch::~IOSSwitch() {}

void IOSSwitch::advance(double timestampSec) {
  const float target = getToggleState() ? 1.0f : 0.0f;

  // The first frame snaps, so the editor doesn't open mid-animation.
  if (lastTimestamp == 0.0) {
    lastTimestamp = timestampSec;
    position = target;
    repaint();
    return;
  }

  const auto elapsed = float(timestampSec - lastTimestamp);
  lastTimestamp = timestampSec;
  if (position == target)
    return;

  // Exponential ease-out; settles in roughly 150 ms.
  position += (target - position) * (1.0f - std::exp(-elapsed / 0.04f));
  if (std::abs(target - position) < 0.002f)
    position = target;
  repaint();
}

void IOSSwitch::paintButton(juce::Graphics &g, bool, bool isDown) {
  const auto bounds =
      getLocalBounds().toFloat().reduced(0.5f).withTrimmedBottom(1.0f);
  const float h = bounds.getHeight();
  const float radius = h * 0.5f;

  juce::Path pill;
  pill.addRoundedRectangle(bounds, radius);

  g.setColour(juce::Colours::white.withAlpha(0.18f));
  g.fillRoundedRectangle(bounds.translated(0.0f, 1.0f), radius);

  const auto thumb = juce::Rectangle<float>(h, h).withPosition(
      bounds.getX() + position * (bounds.getWidth() - h), bounds.getY());
  const float travel = bounds.getWidth() - h;

  {
    // The on/off strip slides with the thumb and is clipped by the pill.
    juce::Graphics::ScopedSaveState state(g);
    g.reduceClipRegion(pill);

    const auto onSide = bounds.withRight(thumb.getCentreX());
    juce::ColourGradient blue(Skin::blueDark, 0.0f, bounds.getY(),
                              Skin::blueLight, 0.0f, bounds.getBottom(), false);
    g.setGradientFill(blue);
    g.fillRect(onSide);

    const auto offSide = bounds.withLeft(thumb.getCentreX());
    g.setGradientFill({juce::Colour(0xffd9d9d9), 0.0f, bounds.getY(),
                       juce::Colour(0xfffbfbfb), 0.0f, bounds.getBottom(),
                       false});
    g.fillRect(offSide);

    const auto font = Skin::font(h * 0.46f).withExtraKerningFactor(0.04f);
    Textures::drawLetterpressText(
        g, onText,
        juce::Rectangle<float>(travel, h).withPosition(thumb.getX() - travel,
                                                       bounds.getY()),
        juce::Justification::centred, juce::Colours::white, font);
    Textures::drawLetterpressText(
        g, offText,
        juce::Rectangle<float>(travel, h).withPosition(thumb.getRight(),
                                                       bounds.getY()),
        juce::Justification::centred, juce::Colour(0xff7a7d82), font);

    Textures::drawInnerShadow(g, pill, juce::Colours::black.withAlpha(0.55f), 4,
                              {0, 2});
  }

  g.setColour(juce::Colours::black.withAlpha(0.5f));
  g.strokePath(pill, juce::PathStrokeType(1.0f));

  // The thumb: a white glossy button that overlaps the track edge.
  juce::Path knob;
  knob.addEllipse(thumb);
  juce::DropShadow(juce::Colours::black.withAlpha(0.45f), 3, {0, 1})
      .drawForPath(g, knob);

  juce::ColourGradient fill(
      isDown ? juce::Colour(0xffdadada) : juce::Colour(0xfff8f8f8), 0.0f,
      thumb.getY(),
      isDown ? juce::Colour(0xffb0b0b0) : juce::Colour(0xffcdcdcd), 0.0f,
      thumb.getBottom(), false);
  g.setGradientFill(fill);
  g.fillPath(knob);
  g.setColour(juce::Colours::black.withAlpha(0.4f));
  g.drawEllipse(thumb.reduced(0.5f), 1.0f);
  g.setGradientFill({juce::Colours::white, 0.0f, thumb.getY(),
                     juce::Colours::transparentWhite, 0.0f, thumb.getCentreY(),
                     false});
  g.drawEllipse(thumb.reduced(1.5f), 1.0f);
}
