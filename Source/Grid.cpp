#include "Grid.h"

USING_NS_AX;

bool Grid::init()
{
    if (!Layer::init())
        return false;

    drawGrid();

    return true;
}

void Grid::setDirection(const SnakeDirection& newDirection)
{
    // Prevent the snake from reversing direction
    if (newDirection.first == -_currentDirection.first && newDirection.second == -_currentDirection.second)
        return;

    _currentDirection = newDirection;
}

char Grid::updateSnake()
{
    const CellPosition nextHeadPos = { _snakeBody.front().first + _currentDirection.first, _snakeBody.front().second + _currentDirection.second };

    // Check for collisions with walls
    if (nextHeadPos.first < 0 || nextHeadPos.first >= GRID_ROWS || nextHeadPos.second < 0 || nextHeadPos.second >= GRID_COLS)
    {
        return -1; // Collision with wall
    }

    Cell* nextHeadCell = _cells[nextHeadPos.first][nextHeadPos.second];

    // Check if food is eaten
    bool ateFood = (nextHeadCell->getState() == CellState::FOOD);
    if (ateFood)
    {
        _score += 10; // Increment score
        spawnFood(); // Spawn new food
    }
    else
    {
        // Move the snake's tail forward (remove the last segment)
        _cells[_snakeBody.back().first][_snakeBody.back().second]->setState(CellState::EMPTY);
        _snakeBody.pop_back();
    }

    // Check for collisions with itself
    if (nextHeadCell->getState() == CellState::SNAKE)
    {
        return -1; // Collision with itself
    }

    // Move the snake's head to the new position
    nextHeadCell->setState(CellState::SNAKE);
    _snakeBody.push_front(nextHeadPos);

    return ateFood ? 1 : 0; // Snake moved successfully
}

void Grid::drawGrid()
{
    const auto origin = Director::getInstance()->getVisibleOrigin();

    _cells.resize(GRID_ROWS);
    for (int i = 0; i < GRID_ROWS; ++i)
    {
        _cells[i].resize(GRID_COLS);
        for (int j = 0; j < GRID_COLS; ++j)
        {
            const auto cell = Cell::create();
            cell->setPosition(origin.x + j * CELL_SIZE, origin.y + i * CELL_SIZE);
            addChild(cell);
            _cells[i][j] = cell;
        }
    }
}

void Grid::spawnFood() const
{
    int row, col;
    do {
        row = RandomHelper::random_int(0, GRID_ROWS - 1);
        col = RandomHelper::random_int(0, GRID_COLS - 1);
    } while (_cells[row][col]->getState() != CellState::EMPTY); // Ensure food is spawned on an empty cell
    _cells[row][col]->setState(CellState::FOOD);
}

void Grid::resetGame()
{
    // Clear the grid
    for (int i = 0; i < GRID_ROWS; ++i)
    {
        for (int j = 0; j < GRID_COLS; ++j)
        {
            _cells[i][j]->setState(CellState::EMPTY);
        }
    }

    _snakeBody.clear();
}

void Grid::setup()
{
    _score = 0;
    _snakeBody.emplace_back(GRID_ROWS / 2, GRID_COLS / 2); // Start in the middle of the grid
    setDirection({0, 1});                                 // Initial direction: moving right
    spawnFood();
}
