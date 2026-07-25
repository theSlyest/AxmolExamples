#include "HudValue.h"

USING_NS_AX;

const Color4F HudValue::BG_COLOR(0.06f, 0.1f, 0.13f, 1.0f);

bool HudValue::init(const std::string& name, const Color4F& color)
{
    if (!DrawNode::init())
    {
        return false;
    }

    _name = name;

    _label = Label::createWithTTF(name + ": 0", "fonts/arial.ttf", 24.0f);    
    Size size = _label->getContentSize() + Size(56.0f, 32.0f);
    setContentSize(size);

    drawSolidRect(Vec2::ZERO, Vec2(size), BG_COLOR);
    float offset = BORDER / 2.0f;
    drawRect(Vec2(offset, offset), Vec2(size.width - offset, size.height - offset), color, BORDER);

    _label->setPosition(size.width / 2.0f, size.height / 2.0f);
    addChild(_label);

    return true;
}

HudValue *HudValue::create(const std::string& name, const Color4F& color)
{
    HudValue *pRet = new(std::nothrow) HudValue();
    if (pRet && pRet->init(name, color))
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

void HudValue::setValue(const std::string &value)
{
    _label->setString(_name + ": " + value);
}
