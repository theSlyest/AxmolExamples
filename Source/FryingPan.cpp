#include "FryingPan.h"

USING_NS_AX;

bool FryingPan::init()
{
    if (!DrawNode::init())
        return false;

    drawSolidCircle(Vec2(Radius, Radius), Radius, 0.0f, 200, Color4F(0.0f, 0.0f, 0.0f, 1.0f));
    drawCircle(Vec2(Radius, Radius), Radius - 4.0f, 0.0f, 200, false, Color4F(0.3f, 0.3f, 0.3f, 1.0f), 8.0f);
    setContentSize(Size(Radius * 2.0f, Radius *2.0f));

    return true;
}
