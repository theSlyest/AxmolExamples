#include "RoundedRect.h"

USING_NS_AX;

bool RoundedRect::init(float width, float height, float radius, const Color4F &color)
{
    if ( !DrawNode::init() )
    {
        return false;
    }

    Vec2 vertices[8] = {
        Vec2(radius, 0.0f),
        Vec2(width - radius, 0.0f),
        Vec2(width, radius),
        Vec2(width, height - radius),
        Vec2(width - radius, height),
        Vec2(radius, height),
        Vec2(0.0f, height - radius),
        Vec2(0.0f, radius)
    };

    drawSolidPoly(vertices, 8, color);

    int segments = 16;
    drawSolidCircle(Vec2(width - radius, radius), radius, 0.0f, segments, color);
    drawSolidCircle(Vec2(width - radius, height - radius), radius, 0.0f, segments, color);
    drawSolidCircle(Vec2(radius, height - radius), radius, 0.0f, segments, color);
    drawSolidCircle(Vec2(radius, radius), radius, 0.0f, segments, color);

    setContentSize(Size(width, height));

    return true;
}

RoundedRect *RoundedRect::create(float width, float height, float radius, const Color4F &color)
{
    RoundedRect *pRet = new(std::nothrow) RoundedRect();
    if (pRet && pRet->init(width, height, radius, color))
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
