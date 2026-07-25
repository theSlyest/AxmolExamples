#include "DiamondShape.h"

USING_NS_AX;

bool DiamondShape::init(const bool isShadow)
{
    if ( !Shape::init(ShapeType::Diamond, isShadow) )
        return false;

    setContentSize(Size(Width, Height));

    return true;
}

DiamondShape* DiamondShape::create(const bool isShadow)
{
    if (auto* pRet = new (std::nothrow) DiamondShape(); pRet && pRet->init(isShadow))
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

ax::Color4F DiamondShape::getShapeColor() { return {0.44f, 0.3f, 0.76f, 1.0f}; }

void DiamondShape::drawType()
{
    constexpr Vec2 points[] = {
        Vec2(0.0f, Height / 2),
        Vec2(Width / 2, 0.0f),
        Vec2(Width, Height / 2),
        Vec2(Width / 2, Height),
    };

    drawPolygon(points, 4, _fillColor, Thickness, _borderColor);
}
