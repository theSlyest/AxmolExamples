#ifndef __LASER_MIRROR_GRID_H__
#define __LASER_MIRROR_GRID_H__

#include "axmol.h"
#include "GridCell.h"
#include "Emitter.h"

class LaserGrid : public ax::DrawNode
{
public:
    static constexpr int ROWS = 8;
    static constexpr int COLS = 12;
    static const ax::Color4F BORDER_COLOR;

    virtual bool init();


    // implement the "static create()" method manually
    CREATE_FUNC(LaserGrid);

    void positionEmitter(LaserSide side, int position);
    
    void reset(const std::vector<std::vector<CellState>>& states);

    bool refreshBeam() ;

    bool onTouch(ax::Touch* touch, bool maxReached);

    int countMirrors() const { return _mirrorCount; }

    bool isTargetReached() const { return _isTargetReached; }

protected:
    static constexpr float BORDER = 4.0f;

    static const std::map<LaserSide, std::pair<int, int>> SHIFTS;

    std::vector<std::vector<GridCell*>> _cells;

    Emitter* _emitter;
    LaserSide _emitterSide;
    int _emitterRow;
    int _emitterCol;

    int _mirrorCount;
    int _isTargetReached;

    void initCells();
    void initEmitter();

    LaserSide invertSide(LaserSide side) const;

    void clearBeam();
};

#endif // __LASER_MIRROR_GRID_H__
