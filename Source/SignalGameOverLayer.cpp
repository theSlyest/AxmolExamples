#include "SignalGameOverLayer.h"

#include "fmt/format.h"

USING_NS_AX;

bool SignalGameOverLayer::init()
{
    if (!LayerColor::initWithColor(Color4B(0, 0, 0, 128)))
        return false;

    setVisible(false);

    addLabel("ROUND OVER", 36.0f, 48.0f);
    _scoreLabel = addLabel("Score: 0", 24.0f, 0.0f);
    addLabel("Press R to restart", 18.0f, -48.0f);

    return true;
}

Label* SignalGameOverLayer::addLabel(const char* text, const float fontSize, const float offsetY)
{
    const auto label = Label::createWithTTF(text, "fonts/arial.ttf", fontSize);
    label->setPosition(_contentSize.width / 2.0f, _contentSize.height / 2.0f + offsetY);
    addChild(label);
    return label;
}

void SignalGameOverLayer::show(const int score)
{
    _scoreLabel->setString(fmt::format("Score: {}", score));
    setVisible(true);
}
