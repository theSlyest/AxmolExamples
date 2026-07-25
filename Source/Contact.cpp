#include "Contact.h"

USING_NS_AX;

bool Contact::init()
{
    if ( !DrawNode::init() )
    {
        return false;
    }

    constexpr float diameter = CON_RADIUS * 2.0f;
    setContentSize(Size(diameter, diameter));
    setAnchorPoint(Vec2::ANCHOR_MIDDLE);

    drawSolidCircle(Vec2(CON_RADIUS, CON_RADIUS), CON_RADIUS, 0.0f, 24, Color4F(1.0f, 0.69f, 0.29f, 1.0f));
    setRandomDirection();

    return true;
}

void Contact::setRandomDirection()
{
    const float randRadAngle = M_PI * rand_minus1_1();
    _direction.x = cos(randRadAngle) * CON_SPEED;
    _direction.y = sin(randRadAngle) * CON_SPEED;
}
