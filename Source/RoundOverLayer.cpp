#include "RoundOverLayer.h"

#include "Theme.h"
#include "fmt/format.h"

USING_NS_AX;

bool RoundOverLayer::init()
{
    if (!LayerColor::initWithColor(Color4B(Theme::BACKGROUND.r, Theme::BACKGROUND.g, Theme::BACKGROUND.b, 192)))
        return false;

    setVisible(false);

    addLabel("ROUND OVER", LINE_SPACING);
    _score = addLabel("Score: 0", 0.0f);
    addLabel("Press R to restart", -LINE_SPACING);

    return true;
}

Label* RoundOverLayer::addLabel(const char* text, const float offsetY)
{
    const auto label = Label::createWithTTF(text, "fonts/arial.ttf", Theme::FONT_SIZE);
    label->setTextColor(Theme::TEXT);
    label->setPosition(_contentSize.width / 2.0f, _contentSize.height / 2.0f + offsetY);
    addChild(label);
    return label;
}

void RoundOverLayer::show(const int score)
{
    _score->setString(fmt::format("Score: {}", score));
    setVisible(true);
}
