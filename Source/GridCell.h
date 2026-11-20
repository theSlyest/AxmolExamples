#pragma once

#include <optional>

#include "2d/DrawNode.h"
#include "2d/Layer.h"
#include "LaserBoard.h"

// How an ordinary cell of the grid looks: a dark square with a border, and
// the beam crossing it. It only draws what it is told to show.
class GridCell : public ax::LayerColor
{
public:
    static constexpr float SIZE = 48.0f;
    static constexpr float BEAM_WIDTH = 6.0f;
    static const ax::Color4B BG_COLOR;
    static const ax::Color4F BORDER_COLOR;
    static const ax::Color4F BEAM_COLOR;

    bool init() override;

    // Show the beam crossing the cell: it comes in through side `from` and goes
    // out through side `to`. Without a `to`, the beam stops in this cell.
    virtual void showBeam(LaserSide from, std::optional<LaserSide> to);

    virtual void clearBeam();

protected:
    ax::DrawNode* _drawNode = nullptr;
    ax::DrawNode* _beamNode = nullptr;

    // Where the beam crosses a side of the cell, a pixel outside it so that
    // neighboring cells join up.
    static ax::Vec2 edgePoint(LaserSide side);
};
