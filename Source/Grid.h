#pragma once

#include "axmol.h"
#include "Square.h"

constexpr int GRID_ROWS = 20;
constexpr int GRID_COLS = 30;
constexpr int START_ROW = 10;
constexpr int START_COL = 2;
constexpr int TARGET_ROW = 10;
constexpr int TARGET_COL = 27;

class Grid : public ax::Node
{
public:
    bool init() override;
    
    // implement the "static create()" method manually
    CREATE_FUNC(Grid);

    void reset() const;

    void click(const ax::Vec2& location) const;

    void setPath(int row, int col) const;

    std::vector<std::vector<bool>> pathFinderGrid() const;

private:
    std::vector<std::vector<Square*>> _squares;

    void addSquare(SquareType type, int row, int col);
};
