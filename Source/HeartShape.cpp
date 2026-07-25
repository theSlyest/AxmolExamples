#include "HeartShape.h"

USING_NS_AX;

bool HeartShape::init(const bool isShadow)
{
    if ( !Shape::init(ShapeType::Heart, isShadow) )
        return false;

    setContentSize(Size(Height, Height));

    return true;
}

HeartShape* HeartShape::create(const bool isShadow)
{
    if (auto* pRet = new (std::nothrow) HeartShape(); pRet && pRet->init(isShadow))
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

ax::Color4F HeartShape::getShapeColor() { return {0.88f, 0.4f, 0.64f, 1.0f}; }

void HeartShape::drawType()
{
    Vec2 leftPoints[49];
    Vec2 rightPoints[49];
    constexpr Vec2 offset(Height / 2.0f, Height / 2.0f);
    for (int i = 0; i < 49; i++) {
        constexpr int halfSegments = 50;
        // Left Half: Map t from PI to 2*PI
        const float tLeft = M_PI + (M_PI * static_cast<float>(i + 1) / static_cast<float>(halfSegments));
        const float xLeft = 16 * powf(sinf(tLeft), 3);
        const float yLeft = 13 * cosf(tLeft) - 5 * cosf(2*tLeft) - 2 * cosf(3*tLeft) - cosf(4*tLeft);
        leftPoints[i] = Vec2(xLeft, yLeft) * Height / 30.0f + offset;
        
        // Right Half: Map t from 0 to PI
        const float tRight = M_PI * static_cast<float>(i + 1) / static_cast<float>(halfSegments);
        const float xRight = 16 * powf(sinf(tRight), 3);
        const float yRight = 13 * cosf(tRight) - 5 * cosf(2*tRight) - 2 * cosf(3*tRight) - cosf(4*tRight);
        rightPoints[i] = Vec2(xRight, yRight) * Height / 30.0f + offset;
    }
        
    // 1. Draw solid fills (No borders on individual halves to avoid central seam line)
    drawSolidPoly(leftPoints, 49, _fillColor);
    drawSolidPoly(rightPoints, 49, _fillColor);
    
    // 2. Draw outer border line manually skipping the central vertical dividing seam
    for (size_t i = 1; i < 49; ++i) {
        drawLine(leftPoints[i - 1], leftPoints[i], _borderColor, Thickness * 2.0f);
        drawLine(rightPoints[i - 1], rightPoints[i], _borderColor, Thickness * 2.0f);
    }
}
