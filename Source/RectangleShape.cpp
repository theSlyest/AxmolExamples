#include "RectangleShape.h"

USING_NS_AX;

bool RectangleShape::init(bool isShadow)
{
    if ( !Shape::init(ShapeType::Rectangle, isShadow) )
        return false;

    setContentSize(Size(Width, Height));

    return true;
}

RectangleShape* RectangleShape::create(bool isShadow)
{
    if (auto* pRet = new (std::nothrow) RectangleShape(); pRet && pRet->init(isShadow))
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

ax::Color4F RectangleShape::getShapeColor() { return {0.90f, 0.62f, 0.16f, 1.0f}; }

void RectangleShape::drawType()
{
    drawSolidRect(Vec2::ZERO, Vec2(Width, Height), _fillColor);
    drawRect(Vec2::ZERO, Vec2(Width, Height), _borderColor, Thickness);
}