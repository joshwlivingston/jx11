#include "SynthPanel.h"
#include "Skin.h"

SynthPanel::SynthPanel() {
  setOpaque(true);
  setSize(Skin::baseWidth, Skin::baseHeight);
}

SynthPanel::~SynthPanel() {}

void SynthPanel::addCaption(juce::Component &target, const juce::String &text) {
  captions.push_back({&target, text});
  repaint();
}

void SynthPanel::paint(juce::Graphics &g) {
  auto bounds = getLocalBounds().toFloat();
  const auto leftCheek = bounds.removeFromLeft(float(Skin::cheekWidth));
  const auto rightCheek = bounds.removeFromRight(float(Skin::cheekWidth));
  const auto header = bounds.removeFromTop(float(Skin::headerHeight));

  paintFaceplate(g, bounds);
  paintHeader(g, header);
  paintCheek(g, leftCheek, leftWood, true);
  paintCheek(g, rightCheek, rightWood, false);
  paintLogo(g, {48.0f, 16.0f, 180.0f, 86.0f});

  const auto font = Skin::font(11.0f).withExtraKerningFactor(0.08f);
  for (const auto &caption : captions) {
    const auto target = caption.target->getBounds().toFloat();
    const auto area = target.withHeight(16.0f).translated(0.0f, -18.0f);
    const bool onLeather = target.getY() < float(Skin::headerHeight);
    Textures::drawLetterpressText(
        g, caption.text.toUpperCase(), area.expanded(20.0f, 0.0f),
        juce::Justification::centred,
        onLeather ? Skin::inkOnLeather : Skin::inkOnMetal, font);
  }
}

void SynthPanel::paintFaceplate(juce::Graphics &g,
                                juce::Rectangle<float> area) {
  metal.draw(g, area, Textures::brushedMetal);

  // A broad reflection of the room light across the upper plate.
  juce::ColourGradient sheen(juce::Colours::white.withAlpha(0.22f), 0.0f,
                             area.getY(), juce::Colours::black.withAlpha(0.06f),
                             0.0f, area.getBottom(), false);
  sheen.addColour(0.45, juce::Colours::transparentWhite);
  g.setGradientFill(sheen);
  g.fillRect(area);

  const float inset = 13.0f;
  Textures::drawScrew(g, {area.getX() + inset, area.getY() + inset}, 4.5f,
                      0.4f);
  Textures::drawScrew(g, {area.getRight() - inset, area.getY() + inset}, 4.5f,
                      1.1f);
  Textures::drawScrew(g, {area.getX() + inset, area.getBottom() - inset}, 4.5f,
                      -0.6f);
  Textures::drawScrew(g, {area.getRight() - inset, area.getBottom() - inset},
                      4.5f, 0.9f);
}

void SynthPanel::paintHeader(juce::Graphics &g, juce::Rectangle<float> area) {
  hide.draw(g, area, Textures::leather);

  // Leather stretched over padding: a soft crown of light along the top,
  // falling off into shadow where it tucks under at the bottom.
  juce::ColourGradient padding(
      juce::Colours::white.withAlpha(0.1f), 0.0f, area.getY(),
      juce::Colours::black.withAlpha(0.35f), 0.0f, area.getBottom(), false);
  padding.addColour(0.35, juce::Colours::transparentWhite);
  padding.addColour(0.8, juce::Colours::transparentBlack);
  g.setGradientFill(padding);
  g.fillRect(area);

  Textures::drawStitching(g, area.reduced(7.0f), 7.0f);

  // The padded header overhangs the faceplate and shades its top edge.
  const auto shadow = area.withY(area.getBottom()).withHeight(10.0f);
  g.setGradientFill({juce::Colours::black.withAlpha(0.45f), 0.0f, shadow.getY(),
                     juce::Colours::transparentBlack, 0.0f, shadow.getBottom(),
                     false});
  g.fillRect(shadow);
  g.setColour(juce::Colours::black.withAlpha(0.7f));
  g.fillRect(area.withY(area.getBottom() - 1.0f).withHeight(1.0f));
}

void SynthPanel::paintCheek(juce::Graphics &g, juce::Rectangle<float> area,
                            Textures::CachedImage &wood, bool isLeft) {
  // Each cheek gets its own cache, so the two pieces of walnut aren't
  // obviously the same plank.
  wood.draw(g, area, [isLeft](int w, int h, float scale) {
    return Textures::walnut(w, h + (isLeft ? 0 : 97), scale)
        .getClippedImage({0, isLeft ? 0 : 97, w, h});
  });

  // Rounded, lacquered edges: lit on the outside, falling into shadow where
  // the cheek meets the panel.
  const float outer = isLeft ? area.getX() : area.getRight();
  const float inner = isLeft ? area.getRight() : area.getX();
  juce::ColourGradient bevel(juce::Colours::black.withAlpha(0.35f), outer, 0.0f,
                             juce::Colours::black.withAlpha(0.45f), inner, 0.0f,
                             false);
  bevel.addColour(0.2, juce::Colours::white.withAlpha(0.14f));
  bevel.addColour(0.5, juce::Colours::transparentWhite);
  bevel.addColour(0.85, juce::Colours::transparentBlack);
  g.setGradientFill(bevel);
  g.fillRect(area);

  g.setGradientFill({juce::Colours::white.withAlpha(0.12f), 0.0f, area.getY(),
                     juce::Colours::transparentWhite, 0.0f,
                     area.getY() + 120.0f, false});
  g.fillRect(area);

  // The cheek casts a shadow onto the panel beside it.
  const float direction = isLeft ? 1.0f : -1.0f;
  const auto shadow = juce::Rectangle<float>(
      isLeft ? inner : inner - 8.0f, area.getY(), 8.0f, area.getHeight());
  g.setGradientFill({juce::Colours::black.withAlpha(0.35f), inner, 0.0f,
                     juce::Colours::transparentBlack, inner + 8.0f * direction,
                     0.0f, false});
  g.fillRect(shadow);
}

void SynthPanel::paintLogo(juce::Graphics &g, juce::Rectangle<float> area) {
  // Power LED: a red dome in a chrome bezel, glowing onto the leather.
  const juce::Point<float> led(area.getX() + 10.0f, area.getY() + 32.0f);
  g.setGradientFill({Skin::ledRed.withAlpha(0.45f), led.x, led.y,
                     Skin::ledRed.withAlpha(0.0f), led.x + 16.0f, led.y, true});
  g.fillEllipse(juce::Rectangle<float>(32.0f, 32.0f).withCentre(led));

  const auto bezel = juce::Rectangle<float>(13.0f, 13.0f).withCentre(led);
  g.setGradientFill({juce::Colour(0xff6f7378), 0.0f, bezel.getY(),
                     juce::Colour(0xffe9ebee), 0.0f, bezel.getBottom(), false});
  g.fillEllipse(bezel);

  const auto dome = bezel.reduced(2.0f);
  g.setGradientFill({Skin::ledRed.brighter(0.6f), led.x, dome.getY() + 2.0f,
                     Skin::ledRed.darker(0.6f), led.x, dome.getBottom(), true});
  g.fillEllipse(dome);
  g.setColour(juce::Colours::white.withAlpha(0.75f));
  g.fillEllipse(dome.getX() + 2.2f, dome.getY() + 1.2f, 3.5f, 2.2f);

  // Chrome nameplate lettering, with a horizon reflection across the middle.
  juce::GlyphArrangement glyphs;
  glyphs.addLineOfText(Skin::font(46.0f).italicised(), "JX11",
                       area.getX() + 26.0f, area.getY() + 48.0f);
  juce::Path logo;
  glyphs.createPath(logo);
  const auto box = logo.getBounds();

  juce::DropShadow(juce::Colours::black.withAlpha(0.9f), 3, {0, 2})
      .drawForPath(g, logo);

  juce::ColourGradient chrome(juce::Colours::white, 0.0f, box.getY(),
                              juce::Colour(0xffe9ecef), 0.0f, box.getBottom(),
                              false);
  chrome.addColour(0.45, juce::Colour(0xffc9cdd2));
  chrome.addColour(0.52, juce::Colour(0xff5f646b));
  chrome.addColour(0.75, juce::Colour(0xffa4aab1));
  g.setGradientFill(chrome);
  g.fillPath(logo);
  g.setColour(juce::Colours::black.withAlpha(0.55f));
  g.strokePath(logo, juce::PathStrokeType(0.8f));

  Textures::drawLetterpressText(
      g, "POLYPHONIC SYNTHESIZER",
      {area.getX() + 2.0f, area.getY() + 56.0f, area.getWidth(), 14.0f},
      juce::Justification::centredLeft, Skin::inkOnLeather.withAlpha(0.75f),
      Skin::font(9.0f).withExtraKerningFactor(0.22f));
}
