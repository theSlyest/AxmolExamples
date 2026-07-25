#include "RoundedRect.h"

USING_NS_AX;

RoundedRect* RoundedRect::create(const Size& size,
                                 const float radius, const Color4F& fillColor, const Color4F& borderColor, const float thickness)
{
    if (auto* pRet = new (std::nothrow) RoundedRect(); pRet && pRet->init(size, radius, fillColor, borderColor, thickness))
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

bool RoundedRect::init(const Size &size, float radius, const Color4F &fillColor, const Color4F &borderColor, const float thickness)
{
    if (!DrawNode::init())
        return false;

    radius = std::min(radius, std::min(size.width, size.height) / 2.0f);
    const int segments = M_PI_2 * radius;
    std::vector<Vec2> points;
    addCornerPoints(points, Vec2(radius, radius), radius, segments, M_PI);
    addCornerPoints(points, Vec2(size.width - radius, radius), radius, segments, -M_PI_2);
    addCornerPoints(points, Vec2(size.width - radius, size.height - radius), radius, segments, 0.0f);
    addCornerPoints(points, Vec2(radius, size.height - radius), radius, segments, M_PI_2);
    drawPolygon(&points[0], points.size(), fillColor, thickness / 2.0f, borderColor);
    setContentSize(size);

    return true;
}

void RoundedRect::addCornerPoints(std::vector<Vec2>& points, const Vec2 &center,
                                  const float radius,
                                  const int segments,
                                  const float startAngle)
{
    for (int i = 0; i <= segments; ++i)
    {
        const float angle = M_PI_2 * i / segments + startAngle;
        points.push_back(center + Vec2(radius * cosf(angle), radius * sinf(angle)));
    }
}
