#include "Cell.h"

USING_NS_AX;

const Color4F Cell::EmptyColor = Color4F(0.149f, 0.169f, 0.212f, 1.0f);
const Color4F Cell::FillColor = Color4F(0.902f, 0.902f, 0.933f, 1.0f);

bool Cell::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !DrawNode::init() )
    {
        return false;
    }

    _state = CellState::Empty;
    drawCell();
    setContentSize(Size(CELL_SIZE, CELL_SIZE));

    return true;
}

void Cell::setState(CellState state)
{
    if (state == CellState::Empty || _state == CellState::Empty)
    {
        _state = state;
        clear();
        drawCell();
    }
}
    
void Cell::leftClick()
{
    if (_state == CellState::Filled)
       setState(CellState::Empty);
    else
       setState(CellState::Filled);
}

void Cell::rightClick()
{
    if (_state == CellState::Blank)
        setState(CellState::Empty);
    else
        setState(CellState::Blank);
}

void Cell::drawCell()
{
    constexpr float thickness = 1.0f;
    drawSolidRect(Vec2::ZERO, Vec2(CELL_SIZE, CELL_SIZE), _state == CellState::Filled ? FillColor : EmptyColor);
    drawRect(Vec2::ZERO, Vec2(CELL_SIZE, CELL_SIZE), Color4F(0.2275f, 0.2549f, 0.3137f, 1.0f), thickness);

    if (_state == CellState::Blank)
    {
        const Color4F crossColor(0.753f, 0.337f, 0.247f, 1.0f);
        constexpr Vec2 points[12] = {
            Vec2(CELL_SIZE / 2.0f - 4.0f, CELL_SIZE / 2.0f),
            Vec2(4.0f, 8.0f),
            Vec2(8.0f, 4.0f),
            Vec2(CELL_SIZE / 2.0f, CELL_SIZE / 2.0f - 4.0f),
            Vec2(CELL_SIZE - 8.0f, 4.0f),
            Vec2(CELL_SIZE - 4.0f, 8.0f),
            Vec2(CELL_SIZE / 2.0f + 4.0f, CELL_SIZE / 2.0f),
            Vec2(CELL_SIZE - 4.0f, CELL_SIZE - 8.0f),
            Vec2(CELL_SIZE - 8.0f, CELL_SIZE - 4.0f),
            Vec2(CELL_SIZE / 2.0f, CELL_SIZE / 2.0f + 4.0f),
            Vec2(8.0f, CELL_SIZE - 4.0f),
            Vec2(4.0f, CELL_SIZE - 8.0f),
        };
        drawSolidPoly(points, 12, crossColor);
    }
}
