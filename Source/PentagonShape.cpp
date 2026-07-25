#include "PentagonShape.h"

USING_NS_AX;

bool PentagonShape::init(const bool isShadow)
{
    if ( !Shape::init(ShapeType::Pentagon, isShadow) )
        return false;

    constexpr float size = Radius * 2.0f;
    setContentSize(Size(size, size));

    return true;
}

PentagonShape* PentagonShape::create(const bool isShadow)
{
    if (auto* pRet = new (std::nothrow) PentagonShape(); pRet && pRet->init(isShadow))
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

ax::Color4F PentagonShape::getShapeColor() { return {0.74f, 0.20f, 0.82f, 1.0f}; }

void PentagonShape::drawType()
{
    constexpr int numPoints = 5;
    constexpr Vec2 center(Radius, Radius);
    constexpr float angleStep = 2.0f * M_PI / numPoints;
    Vec2 points[5];

    for (int i = 0; i < numPoints; ++i) {
        const float angle = i * angleStep + M_PI_2;
        points[i] = center + Vec2(Radius * cosf(angle), Radius * sinf(angle));
    }

    drawPolygon(points, numPoints, _fillColor, Thickness, _borderColor);
}