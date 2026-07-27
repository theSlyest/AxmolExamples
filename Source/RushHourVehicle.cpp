#include "RushHourVehicle.h"

USING_NS_AX;

RushHourVehicle* RushHourVehicle::create(const Color4F& color, const bool isTruck, const bool vertical)
{
    if (auto* pRet = new (std::nothrow) RushHourVehicle(); pRet && pRet->init(color, isTruck, vertical))
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

bool RushHourVehicle::init(const Color4F& color, bool isTruck, bool vertical)
{
    if ( !DrawNode::init() )
        return false;

    _isVertical = vertical;
    _size = isTruck ? 3 : 2;
    const float length = CELL_SIZE * _size;
    const float lengthEnd = length - PADDING - 1.0f;
    constexpr float widthEnd = CELL_SIZE - PADDING - 1.0f;
    constexpr Vec2 start(PADDING, PADDING);

    if (vertical) {
        setContentSize(Size(CELL_SIZE, length));
        drawRoundedRect(start, Vec2(widthEnd, lengthEnd), RADIUS, color);
    }
    else {
        setContentSize(Size(length, CELL_SIZE));
        drawRoundedRect(start, Vec2(lengthEnd, widthEnd), RADIUS, color);
    }

    return true;
}

void RushHourVehicle::drawRoundedRect(const Vec2& origin, const Vec2& destination, float radius, const Color4F& color)
{
    if (radius <= 0.0f) {
        drawSolidRect(origin, destination, color);
        return;
    }

    radius = std::min(radius, std::min(abs(destination.x - origin.x), abs(destination.y - origin.y)));

    const Vec2 points[8] = {
        Vec2(origin.x + radius, origin.y),
        Vec2(destination.x - radius, origin.y),
        Vec2(destination.x, origin.y + radius),
        Vec2(destination.x, destination.y - radius),
        Vec2(destination.x - radius, destination.y),
        Vec2(origin.x + radius, destination.y),
        Vec2(origin.x, destination.y - radius),
        Vec2(origin.x, origin.y + radius)
    };

    drawSolidPoly(points, 8, color);

    const int segments = M_PI * radius;
    drawSolidCircle(Vec2(origin.x + radius, origin.y + radius), radius, 0.0f, segments, color);
    drawSolidCircle(Vec2(destination.x - radius, origin.y + radius), radius, 0.0f, segments, color);
    drawSolidCircle(Vec2(destination.x - radius, destination.y - radius), radius, 0.0f, segments, color);
    drawSolidCircle(Vec2(origin.x + radius, destination.y - radius), radius, 0.0f, segments, color);
}
