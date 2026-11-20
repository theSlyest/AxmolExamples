#include "RippleHud.h"

#include "Theme.h"
#include "fmt/format.h"

USING_NS_AX;

RippleHud* RippleHud::create(const Vec2& size)
{
    if (auto* pRet = new (std::nothrow) RippleHud(); pRet && pRet->init(size))
    {
        pRet->autorelease();
        return pRet;
    }
    else
    {
        delete pRet;
        pRet = nullptr;
        return nullptr;
    }
}

bool RippleHud::init(const Vec2& size)
{
    if (!Node::init())
        return false;

    setContentSize(Size(size));

    _score = addLabel("Score: 0", Vec2::ANCHOR_TOP_LEFT, Vec2(MARGIN, size.y - MARGIN));
    _timer = addLabel("Timer: 45", Vec2::ANCHOR_TOP_RIGHT, Vec2(size.x - MARGIN, size.y - MARGIN));

    return true;
}

Label* RippleHud::addLabel(const char* text, const Vec2& anchor, const Vec2& position)
{
    const auto label = Label::createWithTTF(text, "fonts/arial.ttf", Theme::FONT_SIZE);
    label->setTextColor(Theme::TEXT);
    label->setAnchorPoint(anchor);
    label->setPosition(position);
    addChild(label);
    return label;
}

void RippleHud::setScore(const int score) const
{
    _score->setString(fmt::format("Score: {}", score));
}

void RippleHud::setTimer(const int seconds) const
{
    _timer->setString(fmt::format("Timer: {}", seconds));
}
