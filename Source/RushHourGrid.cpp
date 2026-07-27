#include "RushHourGrid.h"

USING_NS_AX;

const Color4F RushHourGrid::BG_COLOR(0.16f, 0.19f, 0.25f, 1.0f);
const Color4F RushHourGrid::BORDER_COLOR(0.40f, 0.40f, 0.40f, 1.0f);
const Color4F RushHourGrid::LINE_COLOR(0.07f, 0.09f, 0.12f, 1.0f);

bool RushHourGrid::init()
{
    if ( !DrawNode::init() )
    {
        return false;
    }

    constexpr float side = CELL_SIDE * SIZE + 2.0f * BORDER + 2.0f;
    setContentSize(Size(side, side));

    constexpr float offset = BORDER + 1.0f;
    drawSolidRect(Vec2(offset, offset), Vec2(side - offset, side - offset), BG_COLOR);
    constexpr float end = side - offset;
    for (int i = 0; i < SIZE + 1; ++i) {
        const float shift = i * CELL_SIDE;
        drawLine(Vec2(offset + shift, offset), Vec2(offset + shift, end), LINE_COLOR, 1.0f);
        drawLine(Vec2(offset, offset + shift), Vec2(end, offset + shift), LINE_COLOR, 1.0f);
    }

    const auto border = DrawNode::create();
    constexpr float bOffset = BORDER / 2.0f;
    constexpr float bEnd = side - bOffset;
    border->setContentSize(_contentSize);
    border->drawLine(Vec2(bOffset, bOffset), Vec2(bOffset, bEnd), BORDER_COLOR, BORDER);
    border->drawLine(Vec2(bOffset, bOffset), Vec2(bEnd, bOffset), BORDER_COLOR, BORDER);
    border->drawLine(Vec2(bOffset, bEnd), Vec2(bEnd, bEnd), BORDER_COLOR, BORDER);
    border->drawLine(Vec2(bEnd, bOffset), Vec2(bEnd, offset + CELL_SIDE * 3), BORDER_COLOR, BORDER);
    border->drawLine(Vec2(bEnd, offset + CELL_SIDE * 4), Vec2(bEnd, bEnd), BORDER_COLOR, BORDER);
    addChild(border);

    const auto arrow = DrawNode::create();
    arrow->drawLine(Vec2(side, offset + CELL_SIDE * 3.5f), Vec2(side + 40.0f, offset + CELL_SIDE * 3.5f), Color4F::WHITE, 8.0f);
    arrow->drawLine(Vec2(side + 24.0f, offset + CELL_SIDE * 3.7f), Vec2(side + 40.0f, offset + CELL_SIDE * 3.5f), Color4F::WHITE, 8.0f);
    arrow->drawLine(Vec2(side + 24.0f, offset + CELL_SIDE * 3.3f), Vec2(side + 40.0f, offset + CELL_SIDE * 3.5f), Color4F::WHITE, 8.0f);
    addChild(arrow);

    reset();

    return true;
}

GridCell RushHourGrid::getCell(const Vec2 &pos)
{
    constexpr float offset = BORDER + 1.0f;
    return { (pos.x - offset) / CELL_SIDE, (pos.y - offset) / CELL_SIDE};
}

Vec2 RushHourGrid::getCellPosition(const int col, const int row)
{
    constexpr float offset = BORDER + 1.0f;
    return {offset + col * CELL_SIDE, offset + row * CELL_SIDE};
}

Vec2 RushHourGrid::getCellPosition(const GridCell &cell)
{
    return getCellPosition(cell.first, cell.second);
}

void RushHourGrid::freeCells(const GridCell& cell, const int number, const bool vertical)
{
    for (int i = 0; i < number; ++i) {
        if (vertical)
            _cells[cell.first][cell.second + i] = false;        
        else
            _cells[cell.first + i][cell.second] = false;        
    }
}

void RushHourGrid::reset()
{
    _cells.assign(SIZE + 2, std::vector<bool>(SIZE, false));
    for (int c = SIZE; c < SIZE + 2; ++c) {
        for (int r = 0; r < SIZE; ++r) {
            if (r != 3)
                _cells[c][r] = true;
        }
    }
}

Vec2 RushHourGrid::getMinPos(int col, int row, const bool vertical)
{
    GridCell cell { col, row };

    if (vertical)
        --row;
    else
        --col;

    while (col > -1 && row > -1 && !_cells[col][row]) {
        cell = { col, row };

        if (vertical)
            --row;
        else
            --col;
    }

    return getCellPosition(cell);
}

Vec2 RushHourGrid::getMaxPos(int col, int row, const bool vertical, const int size)
{
    GridCell cell { col, row };

    int maxCol;
    int maxRow;
    if (vertical) {
        maxCol = col;
        maxRow = row + size;
    } 
    else {
        maxCol = col + size;
        maxRow = row;
    }

    while (maxCol < SIZE + 2 && maxRow < SIZE && !_cells[maxCol][maxRow]) {
        if (vertical) {
            ++row;
            ++maxRow;
        }
        else {
            ++col;
            ++maxCol;
        }
        cell = { col, row };
    }

    return getCellPosition(cell);
}
