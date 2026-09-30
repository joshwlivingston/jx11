#pragma once

#include <juce_graphics/juce_graphics.h>

// The JX11 visual language: an iOS 6 / GarageBand-era instrument. Walnut end
// cheeks, a stitched leather header, a brushed aluminium faceplate and UIKit
// controls. The light source is directly above, like every iOS 6 surface.
namespace Skin {

// The editor lays everything out on this canvas and scales it as one piece.
inline constexpr int baseWidth = 1100;
inline constexpr int baseHeight = 640;
inline constexpr int cheekWidth = 26;
inline constexpr int headerHeight = 118;

// Section wells: a letterpress title sits on the metal above a recessed well.
inline constexpr int groupTitleHeight = 22;
inline constexpr int groupPadding = 10;
inline constexpr float groupCornerRadius = 10.0f;

// Knobs stop growing at this size; extra room goes around them instead.
inline constexpr int maxKnobDiameter = 96;

// Metal
inline const juce::Colour aluminiumLight{0xffdfe2e5};
inline const juce::Colour aluminiumDark{0xffaeb2b7};

// Leather and stitching
inline const juce::Colour leather{0xff2c221d};
inline const juce::Colour stitch{0xffd9c8a5};

// Walnut
inline const juce::Colour walnutLight{0xff8a5a37};
inline const juce::Colour walnut{0xff5e3b22};
inline const juce::Colour walnutDark{0xff331f12};

// UIKit blue (UISlider fill, selected segment, UISwitch "on")
inline const juce::Colour blueLight{0xff7db4f7};
inline const juce::Colour blue{0xff3b86ec};
inline const juce::Colour blueDark{0xff1b5dc9};
inline const juce::Colour blueEdge{0xff17479a};

// Letterpress ink: dark ink on metal gets a white lip below it, light ink on
// leather gets a dark shadow above it.
inline const juce::Colour inkOnMetal{0xff41464d};
inline const juce::Colour inkOnLeather{0xffe8dcc4};

// Backlit LCD
inline const juce::Colour lcdTop{0xffeef1e2};
inline const juce::Colour lcdBottom{0xffc8cfb4};
inline const juce::Colour lcdInk{0xff262b1f};

inline const juce::Colour ledRed{0xffff3b30};

// Helvetica Neue where it exists (macOS/iOS), Arial elsewhere.
inline juce::Font font(float height, bool bold = true) {
  static const juce::String typeface = [] {
    const auto installed = juce::Font::findAllTypefaceNames();
    for (auto name : {"Helvetica Neue", "Helvetica", "Arial"})
      if (installed.contains(name))
        return juce::String(name);
    return juce::Font::getDefaultSansSerifFontName();
  }();

  return juce::Font{juce::FontOptions{
      typeface, height, bold ? juce::Font::bold : juce::Font::plain}};
}

// The area inside a section well, below its title.
inline juce::Rectangle<int> groupContent(juce::Rectangle<int> bounds) {
  bounds.removeFromTop(groupTitleHeight);
  return bounds.reduced(groupPadding);
}

} // namespace Skin
