#include "PentagoArrow.h"

USING_NS_AX;

const Color4F PentagoArrow::COLOR(Color4B(0xa0, 0xff, 0xff, 0xff));

PentagoArrow *PentagoArrow::create(int orientation)
{
    if (auto* pRet = new (std::nothrow) PentagoArrow(); pRet && pRet->init(orientation))
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

bool PentagoArrow::init(const int orientation)
{
    if (!DrawNode::init())
        return false;

    setContentSize(Vec2(2.0f * RADIUS, 2.0f * RADIUS));

    const auto arc = createArc();
    const auto head = createHead();
    head->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    head->setPosition(orientation % 2 == 0 ? 0.0f : RADIUS * 2.0f, RADIUS * 1.1f);
    arc->addChild(head);
    arc->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    arc->setPosition(RADIUS, RADIUS);
    const float angle = 45.0f + orientation * 90.0f;
    arc->setRotation(angle);
    addChild(arc);

    return true;
}

DrawNode* PentagoArrow::createHead()
{
    constexpr Vec2 size(WIDTH * 3.0f, WIDTH * 2.0f);
    constexpr Vec2 vertices[3] {
        Vec2(0.0f, size.y),
        Vec2(size.x / 2.0f, 0.0f),
        Vec2(size.x, size.y)
    };
    const auto head = DrawNode::create();
    head->setContentSize(size);
    head->drawSolidPoly(vertices, 3, COLOR);
    return head;
}

DrawNode* PentagoArrow::createArc() const
{
    constexpr int segments = RADIUS * M_PI;
    constexpr float coef = 1.0f / RADIUS;
    const auto vertices = new (std::nothrow) Vec2[segments];
    if (!vertices)
        return nullptr;

    for (int i = 0; i < segments; ++i) {
        const float rads = i * coef;
        vertices[i].x = RADIUS * cosf(rads) + RADIUS;
        vertices[i].y = RADIUS * sinf(rads) + RADIUS;
    }

    const auto arc = DrawNode::create();
    arc->setContentSize(_contentSize);
    arc->drawPoly(vertices, segments, false, COLOR, WIDTH / 2.0f);

    return arc;
}
