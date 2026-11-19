#include "BalancerGameOverLayer.h"

#include "2d/Menu.h"
#include "2d/MenuItem.h"
#include "fmt/format.h"

USING_NS_AX;

bool BalancerGameOverLayer::init()
{
    if (!DrawNode::init())
        return false;

    setContentSize(Size(SIZE));
    drawSolidRect(Vec2::ZERO, SIZE, Color4F(0.1f, 0.15f, 0.2f, 1.0f));
    drawRect(Vec2::ZERO, SIZE, Color4F(1.0f, 0.6f, 0.0f, 1.0f), 4.0f);
    setVisible(false);

    const auto out = Label::createWithTTF("Out of strikes", "fonts/arial.ttf", 24.0f);
    out->setTextColor(Color4B(255, 64, 0, 255));
    out->setPosition(SIZE.x / 2.0f, 240.0f);
    addChild(out);

    _upTimeLbl = Label::createWithTTF("Uptime banked: 000s", "fonts/arial.ttf", 18.0f);
    _upTimeLbl->setPosition(SIZE.x / 2.0f, 180.0f);
    addChild(_upTimeLbl);

    _bestLbl = Label::createWithTTF("Best: 000s", "fonts/arial.ttf", 24.0f);
    _bestLbl->setTextColor(Color4B(224, 224, 0, 255));
    _bestLbl->setPosition(SIZE.x / 2.0f, 120.0f);
    addChild(_bestLbl);

    const auto again = Label::createWithTTF("Play Again", "fonts/arial.ttf", 18.0f);
    const auto item = MenuItemLabel::create(again, [this](Object*) { if (onPlayAgain) onPlayAgain(); });
    const Vec2 itemSize = item->getContentSize() + Vec2(48.0f, 24.0f);

    const auto bg = DrawNode::create();
    bg->setContentSize(Size(itemSize));
    bg->drawSolidRect(Vec2::ZERO, itemSize, Color4F(0.25f, 0.3f, 0.35f, 1.0f));
    bg->drawRect(Vec2::ZERO, itemSize, Color4F(0.2f, 0.6f, 1.0f, 1.0f), 3.0f);
    bg->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    bg->setPosition(item->getContentSize() / 2.0f);
    item->addChild(bg, -1);

    const auto menu = Menu::create(item, nullptr);
    menu->setPosition(SIZE.x / 2.0f, 60.0f);
    addChild(menu);

    return true;
}

void BalancerGameOverLayer::show(const int upTime, const int best)
{
    _upTimeLbl->setString(fmt::format("Uptime banked: {:03d}s", upTime));
    _bestLbl->setString(fmt::format("Best: {:03d}s", best));
    setVisible(true);
}
