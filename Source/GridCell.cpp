#include "GridCell.h"

USING_NS_AX;

const Color4B GridCell::BG_COLOR(23, 34, 46, 255);
const Color4F GridCell::BORDER_COLOR(0.12f, 0.17f, 0.22f, 1.0f);
const Color4F GridCell::BEAM_COLOR(0.95f, 0.77f, 0.06f, 1.0f);

bool GridCell::init()
{
    if (!LayerColor::initWithColor(BG_COLOR, SIZE, SIZE))
        return false;

    constexpr Vec2 size(SIZE, SIZE);
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

Vec2 GridCell::edgePoint(const LaserSide side)
{
    switch (side) {
        case LaserSide::LEFT:   return { -1.0f, SIZE / 2.0f };
        case LaserSide::RIGHT:  return { SIZE + 1.0f, SIZE / 2.0f };
        case LaserSide::BOTTOM: return { SIZE / 2.0f, -1.0f };
        case LaserSide::TOP:    return { SIZE / 2.0f, SIZE + 1.0f };
    }

    return Vec2::ZERO;
}

void GridCell::showBeam(const LaserSide from, const std::optional<LaserSide> to)
{
    constexpr Vec2 center(SIZE / 2.0f, SIZE / 2.0f);

    if (!to || *to == LaserBoard::opposite(from)) {
        // Straight through (or, without an exit, as far as the center).
        const Vec2 end = to ? edgePoint(*to) : center;
        _beamNode->drawLine(edgePoint(from), end, BEAM_COLOR, BEAM_WIDTH);
        return;
    }

    // A turn: in to the center, then out.
    _beamNode->drawLine(edgePoint(from), center, BEAM_COLOR, BEAM_WIDTH);
    _beamNode->drawLine(center, edgePoint(*to), BEAM_COLOR, BEAM_WIDTH);
}

void GridCell::clearBeam()
{
    _beamNode->clear();
}
