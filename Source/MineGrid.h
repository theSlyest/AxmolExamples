#ifndef __MINES_GRID_H__
#define __MINES_GRID_H__

#include "axmol.h"
#include "MineCell.h"

class MineGrid : public ax::DrawNode
{
public:
    static constexpr int SIZE = 9;

    virtual bool init();
    
    // implement the "static create()" method manually
    CREATE_FUNC(MineGrid);

    int countCovered() const { return _coveredCount; }
    int countFlags() const { return _flagCoords.size(); }

    void hover(const ax::Vec2& location);
    bool reveal(const ax::Vec2& location); // Returns true if the grid changed
    bool switchFlag(const ax::Vec2& location); // Returns true if the grid changed

    bool populateFrom(const ax::Vec2& location, unsigned int mines);

    bool isMineRevealed() const { return _isMineRevealed; }

    void reset();

private:
    static constexpr float BORDER = 4.0f;
    static constexpr float OFFSET = 12.0f;
    static constexpr float GAP = 4.0f;

    static const ax::Color4F BORDER_COLOR;

    std::set<std::pair<int, int>> _mineCoords;
    std::set<std::pair<int, int>> _flagCoords;

    int _coveredCount;
    bool _isMineRevealed;

    MineCell* _hovered;

    std::vector<std::vector<MineCell*>> _cells;

    void initCells();

    void populateExcept(const std::pair<int, int>& coords, unsigned int mines);

    void revealMines();
    void revealAdjacent(int row, int col);

    void countMines(int row, int col);

    MineCell* locateCell(const ax::Vec2& location);
    std::pair<int, int> locateCoords(const ax::Vec2& location);
};

#endif // __MINES_GRID_H__
