#include "LaserHud.h"

#include "fmt/format.h"

USING_NS_AX;

LaserHud* LaserHud::create(const Vec2& size)
{
    if (auto* pRet = new (std::nothrow) LaserHud(); pRet && pRet->init(size))
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

bool LaserHud::init(const Vec2& size)
{
    if (!Node::init())
        return false;

    setContentSize(Size(size));

    const float top = size.y - TOP;

    _mirrors = HudValue::create("MIRRORS", Color4F(0.21f, 0.6f, 0.86f, 1.0f));
    _mirrors->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
    _mirrors->setPosition(MARGIN, top);
    addChild(_mirrors);

    _remaining = HudValue::create("REMAINING", Color4F(0.36f, 0.67f, 0.31f, 1.0f));
    _remaining->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    _remaining->setPosition(size.x / 2.0f, top);
    addChild(_remaining);

    _best = HudValue::create("BEST", Color4F(0.95f, 0.77f, 0.06f, 1.0f));
    _best->setAnchorPoint(Vec2::ANCHOR_TOP_RIGHT);
    _best->setPosition(size.x - MARGIN, top);
    addChild(_best);

    // How to play, centered at the foot of the screen.
    const auto help = Label::createWithTTF(
        "Click a cell to place a mirror (/), click again to turn it (\\), and once more to remove it.\n"
        "Bend the laser beam into the target using as few mirrors as you can.",
        "fonts/arial.ttf", HELP_FONT_SIZE);
    help->setHorizontalAlignment(TextHAlignment::CENTER);
    help->setTextColor(Color4B(190, 200, 210, 255));
    help->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    help->setPosition(size.x / 2.0f, HELP_BOTTOM);
    addChild(help);

    return true;
}

void LaserHud::setMirrors(const int mirrors) const
{
    _mirrors->setValue(fmt::format("{}", mirrors));
}

void LaserHud::setRemaining(const int remaining) const
{
    _remaining->setValue(fmt::format("{}", remaining));
}

void LaserHud::setBest(const int best) const
{
    _best->setValue(fmt::format("{}", best));
}
