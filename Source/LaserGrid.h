#pragma once

#include "2d/DrawNode.h"
#include "GridCell.h"
#include "Emitter.h"

class LaserGrid : public ax::DrawNode
{
public:
    static constexpr int ROWS = 8;
    static constexpr int COLS = 12;
    static const ax::Color4F BORDER_COLOR;

    bool init() override;


    // implement the "static create()" method manually
    CREATE_FUNC(LaserGrid);

    void positionEmitter(LaserSide side, int position);
    
    void reset(const std::vector<std::vector<CellState>>& states);

    bool refreshBeam() const;

    bool onTouch(ax::Touch* touch, bool maxReached);

    int countMirrors() const { return _mirrorCount; }

    bool isTargetReached() const { return _isTargetReached; }

protected:
    static constexpr float BORDER = 4.0f;

    static const std::map<LaserSide, std::pair<int, int>> SHIFTS;

    std::vector<std::vector<GridCell*>> _cells;

    Emitter* _emitter = nullptr;
    LaserSide _emitterSide = LaserSide::LEFT;
    int _emitterRow = 0;
    int _emitterCol = 0;

    int _mirrorCount = 0;
    int _isTargetReached = 0;

    void initCells();
    void initEmitter();

    LaserSide invertSide(LaserSide side) const;

    void clearBeam() const;
};
