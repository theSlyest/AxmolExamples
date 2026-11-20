#include "Contact.h"

USING_NS_AX;

bool Contact::init()
{
    if (!DrawNode::init())
        return false;

    constexpr float diameter = RADIUS * 2.0f;
    setContentSize(Size(diameter, diameter));
    setAnchorPoint(Vec2::ANCHOR_MIDDLE);

    drawSolidCircle(Vec2(RADIUS, RADIUS), RADIUS, 0.0f, 24, Color4F(1.0f, 0.69f, 0.29f, 1.0f));

    return true;
}
