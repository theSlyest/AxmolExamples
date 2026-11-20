#include "SignalHud.h"

#include "fmt/format.h"

USING_NS_AX;

SignalHud* SignalHud::create(const Vec2& size)
{
    if (auto* pRet = new (std::nothrow) SignalHud(); pRet && pRet->init(size))
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

bool SignalHud::init(const Vec2& size)
{
    if (!Node::init())
        return false;

    setContentSize(Size(size));

    _score = addLabel("Score: 0", Vec2::ANCHOR_TOP_LEFT, Vec2(OFFSET, size.y - OFFSET));
    _time = addLabel("Time: 00", Vec2::ANCHOR_TOP_RIGHT, Vec2(size.x - OFFSET, size.y - OFFSET));

    return true;
}

Label* SignalHud::addLabel(const char* text, const Vec2& anchor, const Vec2& position)
{
    const auto label = Label::createWithTTF(text, "fonts/arial.ttf", FONT_SIZE);
    label->setTextColor(COLOR);
    label->setAnchorPoint(anchor);
    label->setPosition(position);
    addChild(label);
    return label;
}

void SignalHud::setScore(const int score) const
{
    _score->setString(fmt::format("Score: {}", score));
}

void SignalHud::setTime(const int seconds) const
{
    _time->setString(fmt::format("Time: {:2d}", seconds));
}
