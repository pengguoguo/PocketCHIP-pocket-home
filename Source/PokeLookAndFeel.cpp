#include "PokeLookAndFeel.h"
#include "SwitchComponent.h"
#include "Utils.h"

PokeLookAndFeel::PokeLookAndFeel() {
  lightGrey = Colour(0xffe1e1e1);
  medGrey = Colour(0xffc0c0c0);
  chipPink = Colour(0xffeb008b);

  setColour(TextButton::buttonColourId, chipPink);
  setColour(TextButton::buttonOnColourId, chipPink);
  setColour(TextButton::textColourOnId, Colours::white);
  setColour(TextButton::textColourOffId, Colours::white);
  setColour(Slider::backgroundColourId, lightGrey);
  setColour(Slider::thumbColourId, chipPink);
  setColour(TextEditor::backgroundColourId, lightGrey);
  setColour(ListBox::backgroundColourId, chipPink);
  setColour(ListBox::textColourId, Colours::white);
  setColour(SwitchComponent::colorIdBackground, lightGrey);
  setColour(SwitchComponent::colorIdHandle, chipPink);
  setColour(SwitchComponent::colorIdHandleOff, medGrey);

  _232MK = Typeface::createSystemTypefaceFor(BinaryData::_232MKM_TTF, BinaryData::_232MKM_TTFSize);
  roboto = Typeface::createSystemTypefaceFor(BinaryData::RobotoRegular_ttf,
                                             BinaryData::RobotoRegular_ttfSize);
}

PokeLookAndFeel::~PokeLookAndFeel(){};

float PokeLookAndFeel::getDrawableButtonTextHeightForBounds(const Rectangle<int> &bounds) {
  return jmin(20.0f, bounds.getHeight() * 0.25f);
}

float PokeLookAndFeel::getDrawableButtonImageHeightForBounds(const Rectangle<int> &bounds) {
  static const float padding = 5.0f;
  return bounds.getHeight() - (getDrawableButtonTextHeightForBounds(bounds) + padding);
}

Typeface::Ptr PokeLookAndFeel::getTypefaceForFont(const Font &font) {
  return roboto;
}

void PokeLookAndFeel::drawLinearSliderThumb(Graphics &g, int x, int y, int width, int height,
                                            float sliderPos, float minSliderPos, float maxSliderPos,
                                            const Slider::SliderStyle style, Slider &slider) {
  const float radius = getSliderThumbRadius(slider);

  g.setColour(slider.findColour(Slider::thumbColourId));

  float kx, ky;

  if (style == Slider::LinearVertical) {
    kx = x + width * 0.5f;
    ky = sliderPos;
  } else {
    kx = sliderPos;
    ky = y + height * 0.5f;
  }

  Path circle;
  circle.addCentredArc(kx, ky, radius, radius, 0.0f, 0.0f, M_PI * 2.0f);
  circle.closeSubPath();

  g.fillPath(circle);
}

void PokeLookAndFeel::drawLinearSliderBackground(Graphics &g, int x, int y, int width, int height,
                                                 float sliderPos, float minSliderPos,
                                                 float maxSliderPos,
                                                 const Slider::SliderStyle style, Slider &slider) {
  const float radius = std::floor(getSliderThumbRadius(slider) * 0.333f);

  g.setColour(slider.findColour(Slider::backgroundColourId));

  Path indent;

  if (slider.isHorizontal()) {
    const float iy = y + height * 0.5f - radius;
    indent.addRoundedRectangle(x - radius, iy, width + radius * 2.0f, radius * 2.0f, radius);
  } else {
    const float ix = x + width * 0.5f - radius;
    indent.addRoundedRectangle(ix, y - radius, radius * 2.0f, height + radius * 2.0f, radius);
  }

  g.fillPath(indent);
}

void PokeLookAndFeel::drawLinearSlider(Graphics &g, int x, int y, int width, int height,
                                       float sliderPos, float minSliderPos, float maxSliderPos,
                                       const Slider::SliderStyle style, Slider &slider) {
  drawLinearSliderBackground(g, x, y, width, height, sliderPos, minSliderPos, maxSliderPos, style,
                             slider);
  drawLinearSliderThumb(g, x, y, width, height, sliderPos, minSliderPos, maxSliderPos, style,
                        slider);
}

int PokeLookAndFeel::getSliderThumbRadius(Slider &slider) {
  return jmin(14, slider.getHeight() / 2, slider.getWidth() / 2);
}

void PokeLookAndFeel::drawButtonText(Graphics &g, TextButton &button, bool isMouseOverButton,
                                     bool isButtonDown) {
  Font font(getTextButtonFont(button, button.getHeight()));
  g.setFont(font);
  g.setFont(24);
  g.setColour(button.findColour(button.getToggleState() ? TextButton::textColourOnId
                                                        : TextButton::textColourOffId)
                  .withMultipliedAlpha(button.isEnabled() ? 1.0f : 0.5f));

  const int yIndent = jmin(4, button.proportionOfHeight(0.3f));
  const int cornerSize = jmin(button.getHeight(), button.getWidth()) / 2;

  const int fontHeight = roundToInt(font.getHeight() * 0.6f);
  const int leftIndent = jmin(fontHeight, 2 + cornerSize / (button.isConnectedOnLeft() ? 4 : 2));
  const int rightIndent = jmin(fontHeight, 2 + cornerSize / (button.isConnectedOnRight() ? 4 : 2));

  g.drawFittedText(button.getButtonText(), leftIndent, yIndent,
                   button.getWidth() - leftIndent - rightIndent, button.getHeight() - yIndent * 2,
                   Justification::centred, 2);
}

void PokeLookAndFeel::drawButtonBackground(Graphics &g, Button &button,
                                           const Colour &backgroundColour, bool isMouseOverButton,
                                           bool isButtonDown) {
  const int width = button.getWidth();
  const int height = button.getHeight();

  auto path = Path();
  path.addRoundedRectangle(0, 0, width, height, 10);
  g.setColour(chipPink);
  g.fillPath(path);
}

void PokeLookAndFeel::drawDrawableButton(Graphics &g, DrawableButton &button,
                                         bool isMouseOverButton, bool isButtonDown) {
  bool toggleState = button.getToggleState();

  g.fillAll(button.findColour(toggleState ? DrawableButton::backgroundOnColourId
                                          : DrawableButton::backgroundColourId));

  const int textH = (button.getStyle() == DrawableButton::ImageAboveTextLabel)
                        ? getDrawableButtonTextHeightForBounds(button.getLocalBounds())
                        : 0;

  if (textH > 0) {
    g.setFont(textH);

    g.setColour(button.findColour(toggleState ? DrawableButton::textColourOnId
                                              : DrawableButton::textColourId)
                    .withMultipliedAlpha(button.isEnabled() ? 1.0f : 0.4f));

    g.drawFittedText(button.getButtonText(), 2, button.getHeight() - textH - 1,
                     button.getWidth() - 4, textH, Justification::centred, 1);
  }
}
