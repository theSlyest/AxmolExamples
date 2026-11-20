#include "Wall.h"

USING_NS_AX;

const Color4F Wall::WALL_COLOR(0.23f, 0.33f, 0.44f, 1.0f);

bool Wall::init()
{
    if (!GridCell::init())
        return false;

    constexpr float offset = (SIZE - WALL_SIZE) / 2.0f;
    _drawNode->drawSolidRect(Vec2(offset, offset), Vec2(SIZE - offset, SIZE - offset), WALL_COLOR);

    return true;
}

void Wall::showBeam(const LaserSide from, std::optional<LaserSide>)
{
    constexpr float offset = (SIZE - WALL_SIZE) / 2.0f;
    Vec2 end;
    switch (from) {
        case LaserSide::LEFT:   end = Vec2(offset, SIZE / 2.0f); break;
        case LaserSide::RIGHT:  end = Vec2(SIZE - offset, SIZE / 2.0f); break;
        case LaserSide::BOTTOM: end = Vec2(SIZE / 2.0f, offset); break;
        case LaserSide::TOP:    end = Vec2(SIZE / 2.0f, SIZE - offset); break;
    }

    _beamNode->drawLine(edgePoint(from), end, BEAM_COLOR, BEAM_WIDTH);
}
