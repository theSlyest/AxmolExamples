#include "Mirror.h"

USING_NS_AX;

const Color4F Mirror::MIRROR_COLOR(1.0f, 1.0f, 1.0f, 1.0f);

bool Mirror::init()
{
    if (!GridCell::init())
    {
        return false;
    }

    _state = CellState::EMPTY;

    _mirrorNode = DrawNode::create();
    _mirrorNode->setContentSize(Size(SIZE, SIZE));
    addChild(_mirrorNode);

    _isTopLeft = false;
    
    return true;
}

void Mirror::drawMirror()
{
    _mirrorNode->clear();
    float offset = (SIZE - MIRROR_SIZE) / 2.0f;
    if (_isTopLeft)
        _mirrorNode->drawLine(Vec2(offset, SIZE - offset), Vec2(SIZE - offset, offset), MIRROR_COLOR, WIDTH);
    else
        _mirrorNode->drawLine(Vec2(offset, offset), Vec2(SIZE - offset, SIZE - offset), MIRROR_COLOR, WIDTH);
}

void Mirror::onTouch()
{
    _mirrorNode->clear();
    if (_state == CellState::EMPTY) {
        _state = CellState::MIRROR;
        _isTopLeft = false;
        drawMirror();
    }
    else if (_isTopLeft) {
        _state = CellState::EMPTY;
    }
    else {
        _isTopLeft = true;
        drawMirror();
    }
}

bool Mirror::drawReflectedBeam(LaserSide from, LaserSide &to)
{
    Vec2 center(SIZE / 2.0f, SIZE / 2.0f);
    if (_isTopLeft) {
        if (from == LaserSide::LEFT || from == LaserSide::BOTTOM) {
            _beamNode->drawLine(Vec2(-1.0f, SIZE / 2.0f), center, BEAM_COLOR, BEAM_WIDTH);
            _beamNode->drawLine(Vec2(SIZE / 2.0f, -1.0f), center, BEAM_COLOR, BEAM_WIDTH);
            to = (from == LaserSide::LEFT ? LaserSide::BOTTOM : LaserSide::LEFT);
        }
        else {
            _beamNode->drawLine(Vec2(SIZE + 1.0f, SIZE / 2.0f), center, BEAM_COLOR, BEAM_WIDTH);
            _beamNode->drawLine(Vec2(SIZE / 2.0f, SIZE + 1.0f), center, BEAM_COLOR, BEAM_WIDTH);
            to = (from == LaserSide::RIGHT ? LaserSide::TOP : LaserSide::RIGHT);
        }
    }
    else {
        if (from == LaserSide::LEFT || from == LaserSide::TOP) {
            _beamNode->drawLine(Vec2(-1.0f, SIZE / 2.0f), center, BEAM_COLOR, BEAM_WIDTH);
            _beamNode->drawLine(Vec2(SIZE / 2.0f, SIZE + 1.0f), center, BEAM_COLOR, BEAM_WIDTH);
            to = (from == LaserSide::LEFT ? LaserSide::TOP : LaserSide::LEFT);
        }
        else {
            _beamNode->drawLine(Vec2(SIZE + 1.0f, SIZE / 2.0f), center, BEAM_COLOR, BEAM_WIDTH);
            _beamNode->drawLine(Vec2(SIZE / 2.0f, -1.0f), center, BEAM_COLOR, BEAM_WIDTH);
            to = (from == LaserSide::RIGHT ? LaserSide::BOTTOM : LaserSide::RIGHT);
        }
    }

    return true;
}

bool Mirror::drawBeam(LaserSide from, LaserSide &to)
{
    return _state == CellState::EMPTY ? GridCell::drawBeam(from, to) : drawReflectedBeam(from, to);
}
