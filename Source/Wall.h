#pragma once

#include "GridCell.h"

// A wall: a solid block that stops the beam.
class Wall : public GridCell
{
public:
    static constexpr float WALL_SIZE = 44.0f;
    static const ax::Color4F WALL_COLOR;

    bool init() override;

    CREATE_FUNC(Wall);

    // The beam stops at the wall's edge.
    void showBeam(LaserSide from, std::optional<LaserSide> to) override;
};
