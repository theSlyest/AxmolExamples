#include "BalancerGenerator.h"

#include "2d/Menu.h"
#include "2d/MenuItem.h"

USING_NS_AX;

const Size BalancerGenerator::SIZE(48.0f, 260.0f);
const Color4F BalancerGenerator::BG_COLOR(0.35f, 0.35f, 0.35f, 1.0f);
const Color4F BalancerGenerator::BORDER_COLOR(0.85f, 0.85f, 0.85f, 1.0f);

namespace {
// A menu item that shows a triangle pointing up or down. The item itself is a
// blank label: its size is what makes the button easy to hit.
MenuItem* createArrowButton(const bool pointsUp, const Color4F& color, std::function<void()> onClick)
{
    constexpr Size arrowSize(48.0f, 24.0f);
    const float base = pointsUp ? 0.0f : arrowSize.height;
    const float tip = pointsUp ? arrowSize.height : 0.0f;

    const auto label = Label::createWithTTF("  ", "fonts/arial.ttf", 64.0f);
    const auto item = MenuItemLabel::create(label, [onClick = std::move(onClick)](Object*) { onClick(); });

    const auto arrow = DrawNode::create();
    arrow->setContentSize(arrowSize);
    arrow->drawTriangle(Vec2(arrowSize.width / 4.0f, base),
                        Vec2(arrowSize.width / 2.0f, tip),
                        Vec2(arrowSize.width * 3.0f / 4.0f, base),
                        color);
    arrow->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    arrow->setPosition(item->getContentSize() / 2.0f);
    item->addChild(arrow, -1);

    return item;
}
}

BalancerGenerator *BalancerGenerator::create(const Color4F &color, const std::string &name)
{
    if (auto* pRet = new (std::nothrow) BalancerGenerator(); pRet && pRet->init(color, name))
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

bool BalancerGenerator::init(const Color4F &color, const std::string &name)
{
    if(!DrawNode::init())
        return false;

    setContentSize(SIZE);

    // LABEL
    _label = Label::createWithTTF(name, "fonts/arial.ttf", 20.0f);
    _label->setTextColor(Color4B(color));
    _label->setPosition(SIZE.width / 2.0f, 60.0f);
    addChild(_label);

    // PROGRESS BAR
    drawSolidRect(Vec2(0.0f, 80.0f), Vec2(SIZE.width, 80.0f + BAR_HEIGHT), BG_COLOR); // Background
    drawRect(Vec2(0.0f, 80.0f), Vec2(SIZE.width, 80.0f + BAR_HEIGHT), BORDER_COLOR, BORDER); // Border

    // Bar
    _bar = DrawNode::create();
    constexpr float barOffset = BORDER / 2.0f + BAR_PADDING;
    const Size barSize(SIZE.width - 2.0f * barOffset, BAR_HEIGHT - 2.0f * barOffset);
    _bar->setContentSize(barSize);
    _bar->drawSolidRect(Vec2::ZERO, barSize, color);
    _bar->setPosition(barOffset, 80.0f + barOffset);
    addChild(_bar);

    const auto upItem = createArrowButton(true, BORDER_COLOR, [this] { if (onIncrease) onIncrease(); });
    const auto downItem = createArrowButton(false, BORDER_COLOR, [this] { if (onDecrease) onDecrease(); });

    _menu = Menu::create(upItem, downItem, nullptr);
    _menu->alignItemsVerticallyWithPadding(160.0f);
    _menu->setPosition(Vec2(SIZE) / 2.0f + Vec2(0.0f, 16.0f));
    addChild(_menu);

    return true;
}

void BalancerGenerator::setValue(const float value) const
{
    _bar->setScaleY(value);
}

void BalancerGenerator::setEnabled(const bool enabled) const
{
    _menu->setEnabled(enabled);
}
