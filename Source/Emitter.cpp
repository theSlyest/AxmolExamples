#include "Emitter.h"

USING_NS_AX;

const Size Emitter::SIZE(32.0f, 40.0f);
const Color4F Emitter::COLOR(0.91f, 0.3f, 0.23f, 1.0f);
const Color4F Emitter::LIGHT_COLOR(0.95f, 0.77f, 0.06f, 1.0f);

bool Emitter::init()
{
    if (!DrawNode::init())
    {
        return false;
    }

    setContentSize(SIZE);
    drawSolidRect(Vec2::ZERO, Vec2(SIZE), COLOR);
    drawSolidCircle(Vec2(SIZE.width - LIGHT_RADIUS + 1.0f, SIZE.height / 2.0f), LIGHT_RADIUS, 0.0f, LIGHT_RADIUS * M_PI, LIGHT_COLOR);

    return true;
}
