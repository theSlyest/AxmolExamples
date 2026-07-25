#include "HexagonShape.h"

USING_NS_AX;

bool HexagonShape::init(const bool isShadow)
{
    if ( !Shape::init(ShapeType::Hexagon, isShadow) )
        return false;

    constexpr float size = Radius * 2.0f;
    setContentSize(Size(size, size));

    return true;
}

HexagonShape* HexagonShape::create(const bool isShadow)
{
    if (auto* pRet = new (std::nothrow) HexagonShape(); pRet && pRet->init(isShadow))
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

ax::Color4F HexagonShape::getShapeColor() { return {0.28f, 0.4f, 0.88f, 1.0f}; }

void HexagonShape::drawType()
{
    constexpr int numPoints = 6;
    constexpr Vec2 center(Radius, Radius);
    constexpr float angleStep = 2.0f * M_PI / numPoints;
    Vec2 points[6];

    for (int i = 0; i < numPoints; ++i) {
        const float angle = i * angleStep;
        points[i] = center + Vec2(Radius * cosf(angle), Radius * sinf(angle));
    }

    drawPolygon(points, numPoints, _fillColor, Thickness, _borderColor);
}
