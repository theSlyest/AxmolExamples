#include "Mirror.h"

USING_NS_AX;

const Color4F Mirror::MIRROR_COLOR(1.0f, 1.0f, 1.0f, 1.0f);

bool Mirror::init()
{
    if (!GridCell::init())
        return false;

    _mirrorNode = DrawNode::create();
    _mirrorNode->setContentSize(Size(SIZE, SIZE));
    addChild(_mirrorNode);

    return true;
}

void Mirror::setMirror(const bool present, const bool backslash) const
{
    _mirrorNode->clear();
    if (!present)
        return;

    constexpr float offset = (SIZE - MIRROR_SIZE) / 2.0f;
    if (backslash)
        _mirrorNode->drawLine(Vec2(offset, SIZE - offset), Vec2(SIZE - offset, offset), MIRROR_COLOR, WIDTH);
    else
        _mirrorNode->drawLine(Vec2(offset, offset), Vec2(SIZE - offset, SIZE - offset), MIRROR_COLOR, WIDTH);
}
