#include "Cell.h"

USING_NS_AX;

bool Cell::init()
{
    if (!DrawNode::init())
        return false;

    setStatePriv(CellState::EMPTY);
    
    return true;
}

void Cell::drawBorder()
{
    this->drawRect(Vec2::ZERO, Vec2(CELL_SIZE, CELL_SIZE), Color4F::GRAY, CELL_BORDER);
}

void Cell::setStatePriv(const CellState state)
{
    _state = state;
    clear();
    drawBorder();

    constexpr float start = CELL_BORDER + CELL_PADDING;
    constexpr float end = CELL_SIZE - CELL_BORDER - CELL_PADDING;

    switch (state)
    {
        case CellState::EMPTY:
            break;
        case CellState::FOOD:
            drawSolidRect(Vec2(start, start), Vec2(end, end), Color4F::RED);
            break;
        case CellState::SNAKE:
            drawSolidRect(Vec2(start, start), Vec2(end, end), Color4F::GREEN);
            break;
    }
}

void Cell::setState(const CellState state)
{
    if (state == _state)
        return;

    setStatePriv(state);
}
