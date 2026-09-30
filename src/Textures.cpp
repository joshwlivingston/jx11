#include "Textures.h"
#include "Skin.h"
#include <cmath>
#include <vector>

namespace {

// Deterministic lattice noise, so every texture looks the same on every run.
float hash(int x, int y, juce::uint32 seed) {
  auto h = juce::uint32(x) * 374761393u + juce::uint32(y) * 668265263u +
           seed * 2246822519u;
  h = (h ^ (h >> 13)) * 1274126177u;
  h ^= h >> 16;
  return float(h & 0xffffffu) / float(0xffffffu);
}

float valueNoise(float x, float y, juce::uint32 seed) {
  const auto ix = int(std::floor(x));
  const auto iy = int(std::floor(y));
  auto fx = x - float(ix);
  auto fy = y - float(iy);
  fx = fx * fx * (3.0f - 2.0f * fx);
  fy = fy * fy * (3.0f - 2.0f * fy);

  const auto top = juce::jmap(fx, hash(ix, iy, seed), hash(ix + 1, iy, seed));
  const auto bottom =
      juce::jmap(fx, hash(ix, iy + 1, seed), hash(ix + 1, iy + 1, seed));
  return juce::jmap(fy, top, bottom);
}

juce::uint8 channel(float value) {
  return juce::uint8(juce::jlimit(0, 255, juce::roundToInt(value)));
}

// Opaque RGB images are written a row at a time through raw pixel pointers;
// setPixelColour is far too slow for full-panel textures in a debug build.
template <typename Shader>
juce::Image shadeOpaque(int widthPx, int heightPx, Shader &&shade) {
  juce::Image image(juce::Image::RGB, juce::jmax(1, widthPx),
                    juce::jmax(1, heightPx), false);
  juce::Image::BitmapData pixels(image, juce::Image::BitmapData::writeOnly);

  for (int y = 0; y < pixels.height; ++y) {
    auto *line = pixels.getLinePointer(y);
    for (int x = 0; x < pixels.width; ++x) {
      const juce::Colour c = shade(x, y);
      reinterpret_cast<juce::PixelRGB *>(line + x * pixels.pixelStride)
          ->setARGB(255, c.getRed(), c.getGreen(), c.getBlue());
    }
  }
  return image;
}

juce::Colour scaled(juce::Colour c, float factor, float lift = 0.0f) {
  return juce::Colour(channel(c.getRed() * factor + lift),
                      channel(c.getGreen() * factor + lift),
                      channel(c.getBlue() * factor + lift));
}

} // namespace

namespace Textures {

juce::Image brushedMetal(int widthPx, int heightPx, float scale) {
  juce::Random random(0x4a583131);

  // Each row gets its own streak pattern: white noise smeared horizontally,
  // plus a per-row brightness offset. That's what a brushing wheel leaves.
  const int streak = juce::jmax(8, juce::roundToInt(70.0f * scale));
  std::vector<float> noise(size_t(widthPx + streak));
  std::vector<float> rowShade(size_t(juce::jmax(1, heightPx)));
  std::vector<std::vector<float>> rows(size_t(juce::jmax(1, heightPx)));

  for (int y = 0; y < heightPx; ++y) {
    for (auto &n : noise)
      n = random.nextFloat() - 0.5f;

    float sum = 0.0f;
    for (int i = 0; i < streak; ++i)
      sum += noise[size_t(i)];

    auto &row = rows[size_t(y)];
    row.resize(size_t(widthPx));
    const float norm = std::sqrt(float(streak));
    for (int x = 0; x < widthPx; ++x) {
      row[size_t(x)] = sum / norm;
      sum += noise[size_t(x + streak)] - noise[size_t(x)];
    }
    rowShade[size_t(y)] = (random.nextFloat() - 0.5f) * 0.03f;
  }

  return shadeOpaque(widthPx, heightPx, [&](int x, int y) {
    const float t = float(y) / float(juce::jmax(1, heightPx - 1));
    const auto base =
        Skin::aluminiumLight.interpolatedWith(Skin::aluminiumDark, t * 0.8f);
    const float grain = rows[size_t(y)][size_t(x)] * 0.085f;
    const float fine = (hash(x, y, 7) - 0.5f) * 0.02f;
    return scaled(base, 1.0f + grain + fine + rowShade[size_t(y)]);
  });
}

juce::Image leather(int widthPx, int heightPx, float scale) {
  const float pebble = 2.6f * scale;
  const float step = juce::jmax(1.0f, scale);

  auto height = [&](float x, float y) {
    return 0.7f * valueNoise(x / pebble, y / pebble, 11) +
           0.3f * valueNoise(x / (pebble * 0.45f), y / (pebble * 0.45f), 23);
  };

  return shadeOpaque(widthPx, heightPx, [&](int x, int y) {
    const float fx = float(x);
    const float fy = float(y);
    const float h = height(fx, fy);

    // Light from above catches the upper edge of each pebble.
    const float slope = height(fx, fy + step) - h;
    const float mottle =
        valueNoise(fx / (45.0f * scale), fy / (45.0f * scale), 37) - 0.5f;

    return scaled(Skin::leather, 0.72f + 0.45f * h + 0.18f * mottle,
                  slope * 55.0f);
  });
}

juce::Image walnut(int widthPx, int heightPx, float scale) {
  return shadeOpaque(widthPx, heightPx, [&](int x, int y) {
    const float lx = float(x) / scale;
    const float ly = float(y) / scale;

    // Vertical grain that wanders sideways as it runs down the cheek.
    const float warp = 7.0f * valueNoise(lx / 14.0f, ly / 110.0f, 41) +
                       2.0f * std::sin(ly / 41.0f + lx * 0.05f);
    const float ring =
        (lx + warp) / 3.4f + 1.4f * valueNoise(lx / 5.0f, ly / 45.0f, 43);
    const float grain = std::pow(
        0.5f + 0.5f * std::sin(ring * juce::MathConstants<float>::twoPi), 3.0f);

    const float figure = valueNoise(lx / 26.0f, ly / 140.0f, 47);
    const float pore = valueNoise(lx / 0.9f, ly / 16.0f, 53);

    auto c = Skin::walnut.interpolatedWith(Skin::walnutLight, figure * 0.55f)
                 .interpolatedWith(Skin::walnutDark, grain * 0.75f);
    if (pore > 0.78f)
      c = c.interpolatedWith(Skin::walnutDark, (pore - 0.78f) * 3.0f);
    return c;
  });
}

juce::Image knob(float diameter, float skirtRadius, float capRadius,
                 float scale) {
  const int sizePx = juce::jmax(1, juce::roundToInt(diameter * scale));
  juce::Image image(juce::Image::ARGB, sizePx, sizePx, true);
  const juce::Point<float> centre(diameter * 0.5f, diameter * 0.5f);

  auto circle = [&](float radius) {
    return juce::Rectangle<float>(radius * 2.0f, radius * 2.0f)
        .withCentre(centre);
  };

  {
    juce::Graphics g(image);
    g.addTransform(juce::AffineTransform::scale(scale));

    juce::Path skirt;
    skirt.addEllipse(circle(skirtRadius));

    // The knob stands tall off the panel: a soft cast shadow below it and a
    // tight contact shadow where it meets the metal.
    juce::DropShadow(juce::Colours::black.withAlpha(0.55f), 6, {0, 4})
        .drawForPath(g, skirt);
    g.setColour(juce::Colours::black.withAlpha(0.35f));
    g.fillEllipse(circle(skirtRadius + 1.0f).translated(0.0f, 1.0f));

    // Glossy black plastic skirt.
    const auto skirtArea = circle(skirtRadius);
    g.setGradientFill({juce::Colour(0xff4c5056), centre.x, skirtArea.getY(),
                       juce::Colour(0xff08090b), centre.x,
                       skirtArea.getBottom(), false});
    g.fillPath(skirt);

    // Knurled grip: ridges lit from above, fading out towards the bottom.
    const int ridges = juce::jmax(24, juce::roundToInt(skirtRadius * 2.2f));
    for (int i = 0; i < ridges; ++i) {
      const float angle =
          juce::MathConstants<float>::twoPi * float(i) / float(ridges);
      const float light = 0.5f + 0.5f * std::cos(angle);
      const auto inner =
          centre.getPointOnCircumference(capRadius + 1.5f, angle);
      const auto outer =
          centre.getPointOnCircumference(skirtRadius - 0.8f, angle);
      g.setColour(juce::Colours::white.withAlpha(0.04f + 0.16f * light));
      g.drawLine({inner, outer}, 0.8f);
    }

    // Polished rim: bright along the top edge, a faint bounce at the bottom.
    juce::ColourGradient rim(juce::Colours::white.withAlpha(0.55f), centre.x,
                             skirtArea.getY(),
                             juce::Colours::white.withAlpha(0.06f), centre.x,
                             skirtArea.getBottom(), false);
    rim.addColour(0.5, juce::Colours::transparentWhite);
    g.setGradientFill(rim);
    g.drawEllipse(skirtArea.reduced(0.5f), 1.0f);

    // The cap sits proud of the skirt and throws a small shadow onto it.
    juce::Path cap;
    cap.addEllipse(circle(capRadius));
    juce::DropShadow(juce::Colours::black.withAlpha(0.7f), 3, {0, 2})
        .drawForPath(g, cap);
  }

  // Spun aluminium: a conic reflection (bright wedges at 12 and 6 o'clock
  // under a top light) over fine concentric machining rings.
  {
    std::vector<float> rings(size_t(capRadius * scale * 2.0f) + 4);
    juce::Random random(0x5350554e);
    for (auto &r : rings)
      r = (random.nextFloat() - 0.5f) * 0.05f;

    juce::Image::BitmapData pixels(image, juce::Image::BitmapData::readWrite);
    const float capPx = capRadius * scale;
    const float cx = centre.x * scale;
    const float cy = centre.y * scale;

    for (int y = juce::jmax(0, int(cy - capPx - 1));
         y < juce::jmin(pixels.height, int(cy + capPx + 2)); ++y) {
      for (int x = juce::jmax(0, int(cx - capPx - 1));
           x < juce::jmin(pixels.width, int(cx + capPx + 2)); ++x) {
        const float dx = float(x) + 0.5f - cx;
        const float dy = float(y) + 0.5f - cy;
        const float distance = std::sqrt(dx * dx + dy * dy);
        const float coverage =
            juce::jlimit(0.0f, 1.0f, capPx - distance + 0.5f);
        if (coverage <= 0.0f)
          continue;

        const float theta = std::atan2(dx, -dy);
        const float brightness = 0.75f + 0.15f * std::cos(2.0f * theta) +
                                 0.035f * std::cos(6.0f * theta + 0.7f) +
                                 rings[size_t(distance * 2.0f)] -
                                 0.05f * dy / capPx;

        const juce::Colour metal(channel(brightness * 228.0f),
                                 channel(brightness * 233.0f),
                                 channel(brightness * 240.0f));
        pixels.setPixelColour(
            x, y,
            pixels.getPixelColour(x, y).interpolatedWith(metal, coverage));
      }
    }
  }

  {
    juce::Graphics g(image);
    g.addTransform(juce::AffineTransform::scale(scale));

    // Machined chamfer on the cap edge.
    const auto capArea = circle(capRadius);
    g.setGradientFill({juce::Colours::white.withAlpha(0.9f), centre.x,
                       capArea.getY(), juce::Colours::black.withAlpha(0.45f),
                       centre.x, capArea.getBottom(), false});
    g.drawEllipse(capArea.reduced(0.6f), 1.2f);
  }

  return image;
}

void CachedImage::draw(juce::Graphics &g, juce::Rectangle<float> area,
                       const Factory &make) {
  const float scale = g.getInternalContext().getPhysicalPixelScaleFactor();
  const int widthPx = juce::roundToInt(area.getWidth() * scale);
  const int heightPx = juce::roundToInt(area.getHeight() * scale);

  if (!image.isValid() || image.getWidth() != widthPx ||
      image.getHeight() != heightPx)
    image = make(widthPx, heightPx, scale);

  g.setOpacity(1.0f); // drawImage inherits the last colour's alpha
  g.drawImage(image, area);
}

void drawInnerShadow(juce::Graphics &g, const juce::Path &shape,
                     juce::Colour colour, int radius, juce::Point<int> offset) {
  juce::Graphics::ScopedSaveState state(g);
  g.reduceClipRegion(shape);

  // Shadow everything *outside* the shape; the clip leaves only the part
  // that falls inside it.
  juce::Path outside;
  outside.addRectangle(shape.getBounds().expanded(float(radius * 2 + 4)));
  outside.addPath(shape);
  outside.setUsingNonZeroWinding(false);
  juce::DropShadow(colour, radius, offset).drawForPath(g, outside);
}

void drawLetterpressText(juce::Graphics &g, const juce::String &text,
                         juce::Rectangle<float> area,
                         juce::Justification justification, juce::Colour ink,
                         const juce::Font &font) {
  const bool onDark = ink.getPerceivedBrightness() > 0.5f;
  g.setFont(font);

  g.setColour(onDark ? juce::Colours::black.withAlpha(0.8f)
                     : juce::Colours::white.withAlpha(0.75f));
  g.drawText(text, area.translated(0.0f, onDark ? -1.0f : 1.0f), justification,
             false);

  g.setColour(ink);
  g.drawText(text, area, justification, false);
}

void drawGloss(juce::Graphics &g, juce::Rectangle<float> area,
               float cornerRadius, float strength) {
  auto top = area.withHeight(area.getHeight() * 0.5f);
  juce::Path gloss;
  gloss.addRoundedRectangle(top.getX(), top.getY(), top.getWidth(),
                            top.getHeight(), cornerRadius, cornerRadius, true,
                            true, false, false);

  g.setGradientFill({juce::Colours::white.withAlpha(strength), 0.0f, top.getY(),
                     juce::Colours::white.withAlpha(strength * 0.25f), 0.0f,
                     top.getBottom(), false});
  g.fillPath(gloss);
}

void drawStitching(juce::Graphics &g, juce::Rectangle<float> area,
                   float cornerRadius) {
  juce::Path seam;
  seam.addRoundedRectangle(area, cornerRadius);

  // The thread pulls the leather down into a shallow groove.
  g.setColour(juce::Colours::black.withAlpha(0.35f));
  g.strokePath(seam, juce::PathStrokeType(2.4f));

  juce::Path stitches;
  const float dashes[] = {5.0f, 3.5f};
  juce::PathStrokeType(1.5f, juce::PathStrokeType::curved,
                       juce::PathStrokeType::rounded)
      .createDashedStroke(stitches, seam, dashes, 2);

  g.setColour(juce::Colours::black.withAlpha(0.6f));
  g.fillPath(stitches, juce::AffineTransform::translation(0.0f, 1.0f));
  g.setColour(Skin::stitch);
  g.fillPath(stitches);
}

void drawScrew(juce::Graphics &g, juce::Point<float> centre, float radius,
               float rotation) {
  auto head =
      juce::Rectangle<float>(radius * 2.0f, radius * 2.0f).withCentre(centre);

  // Countersunk hole: dark above the head, a lit lip below it.
  g.setColour(juce::Colours::white.withAlpha(0.6f));
  g.fillEllipse(head.expanded(1.2f).translated(0.0f, 0.8f));
  g.setColour(juce::Colours::black.withAlpha(0.45f));
  g.fillEllipse(head.expanded(1.0f));

  g.setGradientFill({juce::Colour(0xfff4f5f6), centre.x, head.getY(),
                     juce::Colour(0xff8d9197), centre.x, head.getBottom(),
                     false});
  g.fillEllipse(head);

  juce::Path cross;
  const float arm = radius * 0.62f;
  cross.addLineSegment({centre.x - arm, centre.y, centre.x + arm, centre.y},
                       1.3f);
  cross.addLineSegment({centre.x, centre.y - arm, centre.x, centre.y + arm},
                       1.3f);
  cross.applyTransform(
      juce::AffineTransform::rotation(rotation, centre.x, centre.y));

  // The lower wall of each slot catches the light.
  g.setColour(juce::Colours::white.withAlpha(0.7f));
  g.fillPath(cross, juce::AffineTransform::translation(0.0f, 0.6f));
  g.setColour(juce::Colour(0xff3a3d42));
  g.fillPath(cross);
}

} // namespace Textures
