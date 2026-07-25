#include "TriangleShape.h"

USING_NS_AX;

bool TriangleShape::init(const bool isShadow)
{
    if ( !Shape::init(ShapeType::Triangle, isShadow) )
        return false;

    const float height = Side * sqrtf(3) / 2;
    setContentSize(Size(Side, height));

    return true;
}

TriangleShape* TriangleShape::create(const bool isShadow)
{
    if (auto* pRet = new (std::nothrow) TriangleShape(); pRet && pRet->init(isShadow))
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

ax::Color4F TriangleShape::getShapeColor() { return {0.44f, 0.68f, 0.26f, 1.0f}; }

void TriangleShape::drawType()
{
    const float height = Side * sqrtf(3) / 2;

    const Vec2 points[] = {
        Vec2::ZERO,
        Vec2(Side, 0.0f),
        Vec2(Side / 2, height),
    };

    drawPolygon(points, 3, _fillColor, Thickness, _borderColor);
}