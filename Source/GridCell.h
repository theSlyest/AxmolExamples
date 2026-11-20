#pragma once

#include "2d/DrawNode.h"
#include "2d/Layer.h"

enum class CellState { EMPTY, MIRROR, WALL, TARGET };
enum class LaserSide { LEFT, BOTTOM, RIGHT, TOP };

class GridCell : public ax::LayerColor
{
public:
    static constexpr float SIZE = 48.0f;
    static constexpr float BEAM_WIDTH = 6.0f;

    static const ax::Color4B BG_COLOR;
    static const ax::Color4F BORDER_COLOR;
    static const ax::Color4F BEAM_COLOR;

    bool init() override;
    
    CellState getState() const { return _state; }

    virtual void onTouch() = 0;

    bool laserOn(LaserSide from, LaserSide& to);
    void laserOff();

    virtual bool isLit() const { return _isLit; }

protected:
    ax::DrawNode* _drawNode = nullptr;
    ax::DrawNode* _beamNode = nullptr;

    CellState _state = CellState::EMPTY;

    bool _isLit = false;

    virtual bool drawBeam(LaserSide from, LaserSide& to);
    virtual void clearBeam();
};
