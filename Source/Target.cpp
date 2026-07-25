#include "Target.h"

#include "Emitter.h"

USING_NS_AX;

const Color4F Target::COLOR(0.61f, 0.35f, 0.71f, 1.0f);

bool Target::init()
{
    if (!GridCell::init())
    {
        return false;
    }

    _state = CellState::TARGET;

    Vec2 center(SIZE / 2.0f, SIZE / 2.0f);
    Size size(SIZE, SIZE);

    _lightNode = DrawNode::create();
    _lightNode->drawCircle(center, LIGHT_RADIUS, 0.0f, LIGHT_RADIUS * M_PI, false, Emitter::LIGHT_COLOR);
    _lightNode->setContentSize(size);
    _lightNode->setVisible(false);
    addChild(_lightNode);
    
    auto circles = DrawNode::create();
    circles->drawSolidCircle(center, INNER_RADIUS, 0.0f, INNER_RADIUS * M_PI, COLOR);
    circles->drawCircle(center, OUTER_RADIUS, 0.0f, OUTER_RADIUS * M_PI, false, COLOR, OUTER_WIDTH);
    circles->setContentSize(size);
    addChild(circles);

    return true;
}

bool Target::drawBeam(LaserSide from, LaserSide &to)
{
    Vec2 center(SIZE / 2.0f, SIZE / 2.0f);
    switch (from) {
        case LaserSide::LEFT:
            _beamNode->drawLine(Vec2(-1.0f, SIZE / 2.0f), center, BEAM_COLOR);
            break;
        case LaserSide::RIGHT:
            _beamNode->drawLine(Vec2(SIZE + 1.0f, SIZE / 2.0f), center, BEAM_COLOR);
            break;
        case LaserSide::BOTTOM:
            _beamNode->drawLine(Vec2(SIZE / 2.0f, -1.0f), center, BEAM_COLOR);
            break;
        case LaserSide::TOP:
            _beamNode->drawLine(Vec2(SIZE / 2.0f, SIZE + 1.0f), center, BEAM_COLOR);
            break;
    }
    
    _lightNode->setVisible(true);
    return false;
}

void Target::clearBeam()
{
    GridCell::clearBeam();
    _lightNode->setVisible(false);
}
