#ifndef __NONOGRAM_CELL_H__
#define __NONOGRAM_CELL_H__

#include "axmol.h"

enum class CellState
{
    Empty,
    Filled,
    Blank
};

class Cell : public ax::DrawNode
{
public:
    static constexpr float CELL_SIZE = 40.0f;

    virtual bool init();
    
    // implement the "static create()" method manually
    CREATE_FUNC(Cell);

    void setState(CellState state);
    CellState getState() const { return _state; }

    void leftClick();
    void rightClick();

private:
    static const ax::Color4F EmptyColor;
    static const ax::Color4F FillColor;

    CellState _state;

    void drawCell();
};

#endif // __NONOGRAM_CELL_H__
