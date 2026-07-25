#include "CircleShape.h"

USING_NS_AX;

bool CircleShape::init(const bool isShadow)
{
    if ( !Shape::init(ShapeType::Circle, isShadow) )
        return false;

    constexpr float diameter = Radius * 2.0f;
    setContentSize(Size(diameter, diameter));

    return true;
}

CircleShape* CircleShape::create(const bool isShadow)
{
    if (auto* pRet = new (std::nothrow) CircleShape(); pRet && pRet->init(isShadow))
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

Color4F CircleShape::getShapeColor() { return {0.82f, 0.26f, 0.20f, 1.0f}; }

void CircleShape::drawType()
{
    drawSolidCircle(Vec2(Radius, Radius), Radius, 0.0f, 160, _fillColor);
    drawCircle(Vec2(Radius, Radius), Radius, 0.0f, 160, false, _borderColor, Thickness);
}
