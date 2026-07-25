#pragma once

#include "Cell.h"
#include "axmol.h"

constexpr int GRID_ROWS = 18;
constexpr int GRID_COLS = 32;

using CellPosition = std::pair<int, int>;
using SnakeDirection = std::pair<char, char>;

class Grid : public ax::Layer
{
public:
    bool init() override;
    
    // implement the "static create()" method manually
    CREATE_FUNC(Grid);

    void spawnFood() const;

    void setDirection(const SnakeDirection& newDirection);

    void resetGame();

    void setup();

    char updateSnake();

    int getScore() const { return _score; }

private:
    std::vector<std::vector<Cell*>> _cells;
    std::deque<CellPosition> _snakeBody; // store the positions of the snake body segments
    CellPosition _foodPosition; // store the current food position as (row, col)
    SnakeDirection _currentDirection; // store the current movement direction of the snake
    int _score = 0;

    void drawGrid();
};
