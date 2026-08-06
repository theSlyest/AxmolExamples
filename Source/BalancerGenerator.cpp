#include "BalancerGenerator.h"

USING_NS_AX;

const Size BalancerGenerator::SIZE(48.0f, 260.0f);
const Color4F BalancerGenerator::BG_COLOR(0.35f, 0.35f, 0.35f, 1.0f);
const Color4F BalancerGenerator::BORDER_COLOR(0.85f, 0.85f, 0.85f, 1.0f);

BalancerGenerator *BalancerGenerator::create(const Color4F &color, const std::string &name, const float speed)
{
    if (auto* pRet = new (std::nothrow) BalancerGenerator(); pRet && pRet->init(color, name, speed))
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

bool BalancerGenerator::init(const Color4F &color, const std::string &name, const float speed)
{
    if(!DrawNode::init())
        return false;

    setContentSize(SIZE);

    // LABEL
    _label = Label::createWithTTF(name, "fonts/arial.ttf", 20.0f);
    _label->setTextColor(Color4B(color));
    _label->setPosition(SIZE.width / 2.0f, 64.0f);
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

    constexpr Size arrowSize(48.0f, 24.0f);
    const auto upLbl = Label::createWithTTF("  ", "fonts/arial.ttf", 64.0f);
    const auto upArrow = DrawNode::create();
    const auto upItem = MenuItemLabel::create(upLbl, [this](Object*) { increaseValue(); });
    upArrow->setContentSize(arrowSize),
    upArrow->drawTriangle(Vec2(arrowSize.width / 4.0f, 0.0f),
                            Vec2(arrowSize.width / 2.0f, arrowSize.height),
                            Vec2(arrowSize.width * 3.0f / 4.0f, 0.0f),
                            BORDER_COLOR);
    upArrow->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    upArrow->setPosition(upItem->getContentSize() / 2.0f);
    upItem->addChild(upArrow, -1);

    const auto downLbl = Label::createWithTTF("  ", "fonts/arial.ttf", 64.0f);
    const auto downArrow = DrawNode::create();
    const auto downItem = MenuItemLabel::create(downLbl, [this](Object*) { decreaseValue(); });
    downArrow->setContentSize(arrowSize),
    downArrow->drawTriangle(Vec2(arrowSize.width / 4.0f, arrowSize.height),
                            Vec2(arrowSize.width / 2.0f, 0.0f),
                            Vec2(arrowSize.width * 3.0f / 4.0f, arrowSize.height),
                            BORDER_COLOR);
    downArrow->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    downArrow->setPosition(downItem->getContentSize() / 2.0f);
    downItem->addChild(downArrow, -1);

    const auto menu = Menu::create(upItem, downItem, nullptr);
    menu->alignItemsVerticallyWithPadding(160.0f);
    menu->setPosition(Vec2(SIZE) / 2.0f + Vec2(0.0f, 16.0f));
    addChild(menu);

    _speed = speed;

    return true;
}

void BalancerGenerator::resetValue(float value)
{
    if (0.0f > value || value > 1.0f)
        return;
    
    _bar->setScaleY(value);
    _goal = value;
}

void BalancerGenerator::update(float dt)
{
    float value = _bar->getScaleY();
    if (value == _goal)
        return;
    
    const float shift = dt * _speed;
    if (value < _goal)
        _bar->setScaleY(std::min(value + shift, _goal));
    else
        _bar->setScaleY(std::max(value - shift, _goal));
}

void BalancerGenerator::decreaseValue()
{
    if (_goal > 0.0f)
        _goal = std::max(0.0f, _goal - STEP);
}

void BalancerGenerator::increaseValue()
{
    if (_goal < 1.0f)
        _goal = std::min(1.0f, _goal + STEP);
}
