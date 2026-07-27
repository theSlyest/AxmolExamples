#pragma once

#include "axmol.h"

using GridCell = std::pair<int, int>;

class RushHourGrid : public ax::DrawNode
{
public:
    static constexpr int SIZE = 6;
    static constexpr float CELL_SIDE = 70.0f;

    bool init() override;
    
    // implement the "static create()" method manually
    CREATE_FUNC(RushHourGrid);

    static GridCell getCell(const ax::Vec2& pos);
    static ax::Vec2 getCellPosition(int col, int row);
    static ax::Vec2 getCellPosition(const GridCell& cell);

    void freeCells(const GridCell& cell, int number, bool vertical);

    void setOccupied(const int col, const int row, const bool occupied) { _cells[col][row] = occupied; }

    void reset();

    ax::Vec2 getMinPos(int col, int row, bool vertical);
    ax::Vec2 getMaxPos(int col, int row, bool vertical, int size);

private:
    static const ax::Color4F BG_COLOR;
    static const ax::Color4F BORDER_COLOR;
    static const ax::Color4F LINE_COLOR;
    static constexpr float BORDER = 12.0f;

    std::vector<std::vector<bool>> _cells;
};
