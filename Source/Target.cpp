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

    constexpr Vec2 center(SIZE / 2.0f, SIZE / 2.0f);
    constexpr Vec2 size(SIZE, SIZE);

    _lightNode = DrawNode::create();
    _lightNode->drawCircle(center, LIGHT_RADIUS, 0.0f, LIGHT_RADIUS * M_PI, false, Emitter::LIGHT_COLOR);
    _lightNode->setContentSize(size);
    _lightNode->setVisible(false);
    addChild(_lightNode);

    const auto circles = DrawNode::create();
    circles->drawSolidCircle(center, INNER_RADIUS, 0.0f, INNER_RADIUS * M_PI, COLOR);
    circles->drawCircle(center, OUTER_RADIUS, 0.0f, OUTER_RADIUS * M_PI, false, COLOR, OUTER_WIDTH);
    circles->setContentSize(size);
    addChild(circles);

    return true;
}

void Target::showBeam(const LaserSide from, std::optional<LaserSide>)
{
    // The beam ends at the center, drawn thin.
    _beamNode->drawLine(edgePoint(from), Vec2(SIZE / 2.0f, SIZE / 2.0f), BEAM_COLOR);
    _lightNode->setVisible(true);
}

void Target::clearBeam()
{
    GridCell::clearBeam();
    _lightNode->setVisible(false);
}
