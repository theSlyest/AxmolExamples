#include "GridCell.h"

USING_NS_AX;

const Color4B GridCell::BG_COLOR(23, 34, 46, 255);
const Color4F GridCell::BORDER_COLOR(0.12f, 0.17f, 0.22f, 1.0f);
const Color4F GridCell::BEAM_COLOR(0.95f, 0.77f, 0.06f, 1.0f);

bool GridCell::init()
{
    if (!LayerColor::initWithColor(BG_COLOR, SIZE, SIZE))
    {
        return false;
    }

    Size size(SIZE, SIZE);
    setContentSize(size);

    _drawNode = DrawNode::create();
    _drawNode->drawRect(Vec2::ZERO, Vec2(SIZE, SIZE), BORDER_COLOR);
    _drawNode->setContentSize(size);
    addChild(_drawNode);

    _beamNode = DrawNode::create();
    _beamNode->setContentSize(size);
    addChild(_beamNode, 1);

    return true;
}

bool GridCell::laserOn(LaserSide from, LaserSide &to)
{
    _isLit = true;
    return drawBeam(from, to);
}

void GridCell::laserOff()
{
    _isLit = false;
    clearBeam();
}

bool GridCell::drawBeam(LaserSide from, LaserSide &to)
{
    if (from == LaserSide::LEFT || from == LaserSide::RIGHT) {
        _beamNode->drawLine(Vec2(-1.0f, SIZE / 2.0f), Vec2(SIZE + 1.0f, SIZE / 2.0f), BEAM_COLOR, BEAM_WIDTH);
        to = from == LaserSide::LEFT ? LaserSide::RIGHT : LaserSide::LEFT;
    }
    else {
        _beamNode->drawLine(Vec2(SIZE / 2.0f, -1.0f), Vec2(SIZE / 2.0f, SIZE + 1.0f), BEAM_COLOR, BEAM_WIDTH);
        to = from == LaserSide::BOTTOM ? LaserSide::TOP : LaserSide::BOTTOM;
    }

    return true;
}

void GridCell::clearBeam()
{
    _beamNode->clear();
}
