#include "MainComponent.h"
#include "PokeLookAndFeel.h"

static const int categoryButtonHeight = 62;
static const int categoryPadding = 10;

static ScopedPointer<DrawableButton> createCategoryButton(const std::string name,
                                                          const char *svgData,
                                                          const char *svgDataSel) {
  ScopedPointer<DrawableButton> button;
  ScopedPointer<Drawable> icon, iconSel;
  ScopedPointer<XmlElement> iconSvg(XmlDocument::parse(svgData));
  ScopedPointer<XmlElement> iconSelSvg(XmlDocument::parse(svgDataSel));

  if (iconSvg != nullptr) icon = Drawable::createFromSVG(*iconSvg);

  if (iconSelSvg != nullptr) iconSel = Drawable::createFromSVG(*iconSelSvg);

  button = new DrawableButton(name, DrawableButton::ImageFitted);
  button->setImages(icon, 0, 0, 0, iconSel);
  button->setRadioGroupId(4444);
  button->setClickingTogglesState(true);
  button->setColour(DrawableButton::backgroundOnColourId, Colour(0xffffffff));

  return button;
};

MainContentComponent::MainContentComponent() {
  lookAndFeel = new PokeLookAndFeel();
  setLookAndFeel(lookAndFeel);

  settingsPage = ScopedPointer<SettingsPageComponent>(new SettingsPageComponent());
  addChildComponent(settingsPage);

  appButton = createCategoryButton("Apps", BinaryData::appsIcon_svg,
                                   BinaryData::appsIconSel_svg);
  appButton->addListener(this);
  addAndMakeVisible(appButton);

  gamesButton = createCategoryButton("Games", BinaryData::gamesIcon_svg,
                                     BinaryData::gamesIconSel_svg);
  gamesButton->addListener(this);
  addAndMakeVisible(gamesButton);

  settingsButton = createCategoryButton("Settings", BinaryData::settingsIcon_svg,
                                        BinaryData::settingsIconSel_svg);
  settingsButton->addListener(this);
  addAndMakeVisible(settingsButton);

  {
    int h = categoryButtonHeight;
    int p = categoryPadding;
    categoryButtonLayout.setItemLayout(0, 0, -1.0, -1.0);
    categoryButtonLayout.setItemLayout(1, h + p, h + p, h);
    categoryButtonLayout.setItemLayout(2, h + p, h + p, h);
    categoryButtonLayout.setItemLayout(3, h + p, h + p, h);
    categoryButtonLayout.setItemLayout(4, 0, -1.0, -1.0);
  }

  closeButton = new TextButton("Close");
  closeButton->setBounds(0, 0, 40, 20);
  addAndMakeVisible(closeButton);
  closeButton->addListener(this);

  setSize(480, 245);
}

MainContentComponent::~MainContentComponent() {}

void MainContentComponent::paint(Graphics &g) {
  g.fillAll(Colours::white);
}

void MainContentComponent::resized() {
  settingsPage->setBounds(getLocalBounds());

  auto bounds = getLocalBounds().reduced(categoryPadding);

  Component *categoryButtons[] = { nullptr,
                                   appButton.get(),
                                   gamesButton.get(),
                                   settingsButton.get(),
                                   nullptr };
  categoryButtonLayout.layOutComponents(categoryButtons, 5, bounds.getX(),
                                        bounds.getY(), bounds.getWidth(),
                                        categoryButtonHeight, false, true);
}

void MainContentComponent::buttonClicked(Button *button) {
  settingsPage->setVisible(false);

  if (button == appButton) {
  } else if (button == gamesButton) {
  } else if (button == settingsButton) {
    settingsPage->setVisible(true);
  } else if (button == closeButton) {
    JUCEApplication::quit();
  }
}
