#include "SquareShape.h"

USING_NS_AX;

bool SquareShape::init(const bool isShadow)
{
    if ( !Shape::init(ShapeType::Square, isShadow) )
        return false;

    setContentSize(Size(Side, Side));

    return true;
}

SquareShape* SquareShape::create(const bool isShadow)
{
    if (auto* pRet = new (std::nothrow) SquareShape(); pRet && pRet->init(isShadow))
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

ax::Color4F SquareShape::getShapeColor() { return {0.26f, 0.4f, 0.92f, 1.0f}; }

void SquareShape::drawType()
{
    drawSolidRect(Vec2::ZERO, Vec2(Side, Side), _fillColor);
    drawRect(Vec2::ZERO, Vec2(Side, Side), _borderColor, Thickness);
}