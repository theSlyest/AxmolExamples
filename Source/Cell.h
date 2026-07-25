#pragma once

#include "axmol.h"

constexpr float CELL_SIZE = 40.0f;
constexpr float CELL_BORDER = 1.0f;
constexpr float CELL_PADDING = 1.0f;

enum class CellState { EMPTY, FOOD, SNAKE };

class Cell : public ax::DrawNode
{
public:
    bool init() override;
    CREATE_FUNC(Cell);

    void setState(CellState state);
    CellState getState() const { return _state; }

private:
    CellState _state = CellState::EMPTY;

    void setStatePriv(CellState state);
    void drawBorder();
};
