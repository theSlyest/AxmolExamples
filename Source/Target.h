#pragma once

#include "GridCell.h"

class Target : public GridCell
{
public:
    static const ax::Color4F COLOR;
    static const ax::Color4F LIGHT_COLOR;

    bool init() override;
    
    // implement the "static create()" method manually
    CREATE_FUNC(Target);

    // The beam ends at the target and lights it up.
    void showBeam(LaserSide from, std::optional<LaserSide> to) override;
    void clearBeam() override;

private:
    static constexpr float INNER_RADIUS = 6.0f;
    static constexpr float OUTER_RADIUS = 15.0f;
    static constexpr float OUTER_WIDTH = 6.0f;
    static constexpr float LIGHT_RADIUS = 21.0f;

    ax::DrawNode* _lightNode = nullptr;
};
