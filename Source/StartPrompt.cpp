#include "StartPrompt.h"

#include "2d/Label.h"
#include "Theme.h"

USING_NS_AX;

bool StartPrompt::init()
{
    if (!LayerColor::initWithColor(Theme::BACKGROUND))
        return false;

    const auto label = Label::createWithTTF("Click the surface to send a ripple", "fonts/arial.ttf", Theme::FONT_SIZE);
    label->setTextColor(Theme::TEXT);
    const Size size = label->getContentSize() + Size(PADDING, PADDING);

    setContentSize(size);
    label->setPosition(size / 2.0f);
    addChild(label);

    return true;
}

void StartPrompt::centerIn(const Size& size)
{
    setPosition((size - getContentSize()) / 2.0f);
}
