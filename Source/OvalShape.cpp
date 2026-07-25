#include "OvalShape.h"

USING_NS_AX;

bool OvalShape::init(const bool isShadow)
{
    if ( !Shape::init(ShapeType::Oval, isShadow) )
        return false;

    constexpr float diameter = Radius * 2.0f;
    setContentSize(Size(diameter, diameter));

    return true;
}

OvalShape* OvalShape::create(bool isShadow)
{
    if (auto* pRet = new (std::nothrow) OvalShape(); pRet && pRet->init(isShadow))
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

ax::Color4F OvalShape::getShapeColor() { return {0.4f, 0.7f, 0.74f, 1.0f}; }

void OvalShape::drawType()
{
    constexpr float scaleY = SmallRadius / Radius;
    drawSolidCircle(Vec2(Radius, Radius), Radius, 0.0f, 160, 1.0f, scaleY, _fillColor);
    drawCircle(Vec2(Radius, Radius), Radius, 0.0f, 160, false, 1.0f, scaleY, _borderColor, Thickness);
}