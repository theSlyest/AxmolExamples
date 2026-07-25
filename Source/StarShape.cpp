#include "StarShape.h"

USING_NS_AX;

bool StarShape::init(bool isShadow)
{
    if ( !Shape::init(ShapeType::Star, isShadow) )
        return false;

    constexpr float size = Radius * 2.0f;
    setContentSize(Size(size, size));

    return true;
}

StarShape* StarShape::create(bool isShadow)
{
    if (auto* pRet = new (std::nothrow) StarShape(); pRet && pRet->init(isShadow))
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

ax::Color4F StarShape::getShapeColor() { return {0.94f, 0.8f, 0.2f, 1.0f}; }

void StarShape::drawType()
{
    constexpr int numPoints = 10;
    constexpr Vec2 center(Radius, Radius);
    constexpr float angleStep = 2 * M_PI / numPoints;
    Vec2 points[10];

    for (int i = 0; i < numPoints; ++i) {
        const float r = (i % 2 == 1) ? Radius : InnerRadius;
        const float angle = M_PI_2 + (i - 1) * angleStep;
        points[i] = center + Vec2(r * cosf(angle), r * sinf(angle));
    }

    drawPolygon(points, numPoints, _fillColor, Thickness, _borderColor);
}