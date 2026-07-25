#include "Grid.h"

USING_NS_AX;

// on "init" you need to initialize your instance
bool Grid::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Node::init() )
    {
        return false;
    }

    _squares.assign(GRID_ROWS, std::vector<Square*>(GRID_COLS, nullptr));
    for (int row = 0; row < GRID_ROWS; ++row) {
        for (int col = 0; col < GRID_COLS; ++col) {
            SquareType type = SquareType::Empty;
            if (row == START_ROW && col == START_COL) {
                type = SquareType::Start;
            } else if (row == TARGET_ROW && col == TARGET_COL) {
                type = SquareType::Target;
            }
            addSquare(type, row, col);
        }
    }

    return true;
}

void Grid::addSquare(const SquareType type, const int row, const int col)
{
    _squares[row][col] = Square::create(type);
    _squares[row][col]->setPosition(col * SQUARE_SIZE, row * SQUARE_SIZE);
    addChild(_squares[row][col]);
}

void Grid::reset() const
{
    for (int row = 0; row < GRID_ROWS; ++row) {
        for (int col = 0; col < GRID_COLS; ++col) {
            if (!(row == START_ROW && col == START_COL) && !(row == TARGET_ROW && col == TARGET_COL))
                _squares[row][col]->setType(SquareType::Empty);
        }
    }
}

void Grid::click(const ax::Vec2& location) const
{
    int row = int(location.y / SQUARE_SIZE);
    int col = int(location.x / SQUARE_SIZE);
    _squares[row][col]->switchType();
}

std::vector<std::vector<bool>> Grid::pathFinderGrid() const
{
    std::vector<std::vector<bool>> res(GRID_ROWS, std::vector<bool>(GRID_COLS, false));
    for (int row = 0; row < GRID_ROWS; ++row) {
        for (int col = 0; col < GRID_COLS; ++col) {
            if (_squares[row][col]->getType() == SquareType::Wall)
                res[row][col] = true;
        }
    }
    return res;
}

void Grid::setPath(const int row, const int col) const
{
    _squares[row][col]->setType(SquareType::Path);
}
