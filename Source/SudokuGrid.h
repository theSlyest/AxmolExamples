#ifndef __SUDOKU_GRID_H__
#define __SUDOKU_GRID_H__

#include "axmol.h"
#include "GridCell.h"

class SudokuGrid : public ax::Node
{
public:
    static constexpr int SIDE = 9;
    static constexpr int GROUP_SIDE = 3;
    static constexpr float BORDER = 3.0f;

    virtual bool init();

    // implement the "static create()" method manually
    CREATE_FUNC(SudokuGrid);

    GridCell *click(const ax::Vec2 &worldPoint);

    bool setDigit(const char digit);
    bool erase();

    void loadClues(const std::vector<std::vector<char>>& clues);

    int countLeft() const { return _leftCount; }

    void clear();
    void deselect();

    bool checkConflict();
    bool checkAll();

private:
    static const ax::Color4F LineColorCell;
    static const ax::Color4F LineColorBorder;

    int _leftCount;
    std::vector<std::vector<GridCell*>> _cells;
    GridCell* _selected;
    int _row;
    int _col;

    void initCells();
    void initLines();

    bool checkSelected();
    bool checkCell(const int row, const int col);

    void refreshConflict(const char digit);
    bool isConflicted(const int row, const int col);
};

#endif // __SUDOKU_GRID_H__
