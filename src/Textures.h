#pragma once

#include <functional>
#include <juce_graphics/juce_graphics.h>

// Procedural materials and the small drawing vocabulary shared by every
// control: inner shadows, letterpress text, gloss, stitching and screws.
//
// Texture generators take a size in physical pixels plus the physical scale
// (pixels per logical unit), so they can put detail on real pixels.
namespace Textures {

juce::Image brushedMetal(int widthPx, int heightPx, float scale);
juce::Image leather(int widthPx, int heightPx, float scale);
juce::Image walnut(int widthPx, int heightPx, float scale);

// A knob body (drop shadow, black knurled skirt, spun-aluminium cap), drawn
// in a square of `diameter` logical units. The spun-metal reflection is fixed
// to the light, so this never rotates; only the indicator drawn on top does.
juce::Image knob(float diameter, float skirtRadius, float capRadius,
                 float scale);

// Keeps an image rendered at the destination's physical resolution, so that
// cached artwork stays crisp at any editor scale and on HiDPI screens.
class CachedImage {
public:
  using Factory =
      std::function<juce::Image(int widthPx, int heightPx, float scale)>;

  void draw(juce::Graphics &g, juce::Rectangle<float> area,
            const Factory &make);

private:
  juce::Image image;
};

void drawInnerShadow(juce::Graphics &g, const juce::Path &shape,
                     juce::Colour colour, int radius, juce::Point<int> offset);

// Dark ink gets a white lip below it (pressed into metal); light ink gets a
// dark shadow above it (printed on a dark surface).
void drawLetterpressText(juce::Graphics &g, const juce::String &text,
                         juce::Rectangle<float> area,
                         juce::Justification justification, juce::Colour ink,
                         const juce::Font &font);

// The top-half glass highlight found on every iOS 6 bar and button.
void drawGloss(juce::Graphics &g, juce::Rectangle<float> area,
               float cornerRadius, float strength);

void drawStitching(juce::Graphics &g, juce::Rectangle<float> area,
                   float cornerRadius);

void drawScrew(juce::Graphics &g, juce::Point<float> centre, float radius,
               float rotation);

} // namespace Textures
