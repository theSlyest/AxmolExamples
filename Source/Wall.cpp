#include "Wall.h"

USING_NS_AX;

const Color4F Wall::WALL_COLOR(0.23f, 0.33f, 0.44f, 1.0f);

bool Wall::init()
{
    if (!GridCell::init())
    {
        return false;
    }

    _state = CellState::WALL;

    float offset = (SIZE - WALL_SIZE) / 2.0f;
    _drawNode->drawSolidRect(Vec2(offset, offset), Vec2(SIZE - offset, SIZE - offset), WALL_COLOR);

    return true;
}

bool Wall::drawBeam(LaserSide from, LaserSide &to)
{
    float offset = (SIZE - WALL_SIZE) / 2.0f;
    switch (from) {
        case LaserSide::LEFT:
            _beamNode->drawLine(Vec2(-1.0f, SIZE / 2.0f), Vec2(offset, SIZE / 2.0f), BEAM_COLOR, BEAM_WIDTH);
            break;
        case LaserSide::RIGHT:
            _beamNode->drawLine(Vec2(SIZE + 1.0f, SIZE / 2.0f), Vec2(SIZE - offset, SIZE / 2.0f), BEAM_COLOR, BEAM_WIDTH);
            break;
        case LaserSide::BOTTOM:
            _beamNode->drawLine(Vec2(SIZE / 2.0f, -1.0f), Vec2(SIZE / 2.0f, offset), BEAM_COLOR, BEAM_WIDTH);
            break;
        case LaserSide::TOP:
            _beamNode->drawLine(Vec2(SIZE / 2.0f, SIZE + 1.0f), Vec2(SIZE / 2.0f, SIZE - offset), BEAM_COLOR, BEAM_WIDTH);
            break;
    }
    return false;
}
