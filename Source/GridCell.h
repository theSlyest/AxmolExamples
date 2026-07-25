#ifndef __LASER_MIRROR_CELL_H__
#define __LASER_MIRROR_CELL_H__

#include "axmol.h"

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

    virtual bool init();
    
    CellState getState() const { return _state; }

    virtual void onTouch() = 0;

    bool laserOn(LaserSide from, LaserSide& to);
    void laserOff();

    bool isLit() const { return _isLit; }

protected:
    ax::DrawNode* _drawNode;
    ax::DrawNode* _beamNode;

    CellState _state;

    bool _isLit;

    virtual bool drawBeam(LaserSide from, LaserSide& to);
    virtual void clearBeam();
};

#endif // __LASER_MIRROR_CELL_H__
