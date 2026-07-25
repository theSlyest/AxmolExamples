#include "Destination.h"

USING_NS_AX;

const Size Destination::SIZE(60.0f, 60.0f);

bool Destination::init() 
{
    if (!DrawNode::init())
        return false;

    setContentSize(SIZE);
    drawSolidRect(Vec2::ZERO, Vec2(SIZE), Color4F::YELLOW);

    return true;
}
