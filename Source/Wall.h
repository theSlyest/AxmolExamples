#pragma once

#include "GridCell.h"

class Wall : public GridCell
{
public:
    static constexpr float WALL_SIZE = 44.0f;
    static const ax::Color4F WALL_COLOR;

    bool init() override;
    
    // implement the "static create()" method manually
    CREATE_FUNC(Wall);

    void onTouch() override {}

protected:
    bool drawBeam(LaserSide from, LaserSide& to) override;
};
