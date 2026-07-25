#include "BaseBox.h"

USING_NS_AX;

BaseBox* BaseBox::create(const BoxType type, const int width, const int height)
{
    if (auto pRet = new (std::nothrow) BaseBox(); pRet && pRet->init(type, width, height))
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

bool BaseBox::init(const BoxType type, const int width, const int height)
{
    if (!DrawNode::init())
        return false;

    _boxType = type;
    const Color4F color = typeColor(type);
    drawBox(color, convertDiscreteValue(width), convertDiscreteValue(height));
    setAnchorPoint(Vec2::ZERO);

    return true;
}

float BaseBox::convertDiscreteValue(const int val) { return val * BASE_LENGTH; }

Color4F BaseBox::typeColor(const BoxType type)
{
    switch (type) {
        case BoxType::Obstacle:
            return Color4F::RED;
        case BoxType::Player:
            return Color4F::BLUE;
        default:
            return Color4F::GREEN;
    }
}

void BaseBox::setDiscretePosition(const int x, const int y) { setPosition(convertDiscreteValue(x), convertDiscreteValue(y)); }

void BaseBox::drawBox(const Color4F color, const float width, const float height)
{
    setContentSize(Size(width, height));
    drawSolidRect(Vec2::ZERO, Vec2(width, height), color);
}

int BaseBox::getDiscretePositionX() const { return _position.x / BASE_LENGTH; }

int BaseBox::getDiscretePositionY() const { return _position.y / BASE_LENGTH; }

int BaseBox::getDiscreteWidth() const { return _contentSize.width / BASE_LENGTH; }