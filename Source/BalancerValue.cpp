#include "BalancerValue.h"

USING_NS_AX;

Size BalancerValue::PADDING(64.0f, 32.0f);
Color4F BalancerValue::BG_COLOR(0.1f, 0.1f, 0.2f, 1.0f);

BalancerValue *BalancerValue::create(const Color4F &color, const std::string &name)
{
    if (auto pRet = new (std::nothrow) BalancerValue(); pRet && pRet->init(color, name))
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

bool BalancerValue::init(const Color4F &color, const std::string &name)
{
    if(!DrawNode::init())
        return false;

    _name = name;

    _label = Label::createWithTTF(name + ": 0000", "fonts/arial.ttf", 24.0f);
    _label->setHorizontalAlignment(TextHAlignment::CENTER);
    
    const Size size = _label->getContentSize() + PADDING;
    setContentSize(size);
    _label->setPosition(size / 2.0f);
    addChild(_label);

    drawSolidRect(Vec2::ZERO, size, BG_COLOR);
    drawRect(Vec2::ZERO, size, color, BORDER);

    return true;
}
