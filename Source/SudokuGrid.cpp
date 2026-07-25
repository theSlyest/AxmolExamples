#include "SudokuGrid.h"

USING_NS_AX;

const Color4F SudokuGrid::LineColorCell(0.7176f, 0.6706f, 0.549f, 1.0f);
const Color4F SudokuGrid::LineColorBorder(0.7176f, 0.6706f, 0.549f, 1.0f);

bool SudokuGrid::init()
{
    if (!Node::init())
    {
        return false;
    }

    _leftCount = SIDE * SIDE;
    float side = BORDER * 2.0f + GridCell::SIZE * SIDE;
    setContentSize(Size(side, side));

    initCells();
    initLines();

    _selected = nullptr;
    _row = -1;
    _col = -1;

    return true;
}

void SudokuGrid::initCells()
{
    for (int row = 0; row < SIDE; ++row) {
        std::vector<GridCell*> gridRow;
        bool isDeeper = (row / GROUP_SIDE != 1);
        float y = BORDER + row * GridCell::SIZE;
        for (int col = 0; col < SIDE; ++col) {
            if (col % 3 == 0)
                isDeeper = !isDeeper;

            auto cell = GridCell::create(isDeeper);
            cell->setPosition(BORDER + col * GridCell::SIZE, y);
            addChild(cell);
            gridRow.push_back(cell);
        }
        _cells.push_back(gridRow);
    }
}

void SudokuGrid::initLines()
{
    float length = GridCell::SIZE * SIDE;
    auto lines = DrawNode::create();
    for (int i = 1; i < 9; ++i) {
        lines->drawLine(Vec2(BORDER, BORDER + i * GridCell::SIZE), Vec2(BORDER + length, BORDER + i * GridCell::SIZE), LineColorCell, 1.0f);
        lines->drawLine(Vec2(BORDER + i * GridCell::SIZE, BORDER), Vec2(BORDER + i * GridCell::SIZE, BORDER + length), LineColorCell, 1.0f);
    }
    addChild(lines);

    const float offset = BORDER / 2.0f;
    auto border = DrawNode::create();
    border->drawRect(Vec2(offset, offset), Vec2(length + BORDER, length + BORDER), LineColorBorder, BORDER);

    border->setContentSize(Size(length + 2.0f * BORDER, length + 2.0f * BORDER));
    addChild(border);
}

GridCell *SudokuGrid::click(const Vec2 &worldPoint)
{
    const Vec2& point = convertToNodeSpace(worldPoint);
    for (int row = 0; row < SIDE; ++row) {
        for (int col = 0; col < SIDE; ++col) {
            GridCell* cell = _cells[row][col];
            if (cell->getBoundingBox().containsPoint(point)) {
                if (_selected != cell) {
                    if (_selected != nullptr)
                        _selected->deselect();

                    _row = row;
                    _col = col;
                    _selected = cell;
                    cell->select();
                }
                return cell;
            }
        }
    }

    deselect();

    return nullptr;
}

void SudokuGrid::deselect()
{
    if (_selected != nullptr)
        _selected->deselect();

    _row = -1;
    _col = -1;
    _selected = nullptr;
}

bool SudokuGrid::setDigit(const char digit)
{
    if (_selected == nullptr || _selected->isClue())
        return false;

    if (digit < '1' || digit > '9')
        return false;

    const char old = _selected->getDigit();
    if (old == digit)
        return false;

    _selected->setDigit(digit);

    if (old == 0)
        --_leftCount;
    else
        refreshConflict(old);

    return true;
}

bool SudokuGrid::erase()
{
    if (_selected == nullptr || _selected->isClue())
        return false;

    const char old = _selected->getDigit();
    if (old == 0)
        return false;
    
    ++_leftCount;
    _selected->clear();
    refreshConflict(old);

    return true;
}

void SudokuGrid::loadClues(const std::vector<std::vector<char>> &clues)
{
    for (int r = 0; r < SIDE; ++r) {
        for (int c = 0; c < SIDE; ++c) {
            if (clues[r][c] > 0) {
                _cells[r][c]->setClueDigit(clues[r][c]);
                --_leftCount;
            }
        }
    }
}

void SudokuGrid::clear()
{
    for (auto& row : _cells) {
        for (GridCell* cell : row) {
            cell->clear();
        }
    }

    deselect();
    _leftCount = SIDE * SIDE;
}

bool SudokuGrid::checkConflict()
{
    bool hasConflict = checkSelected();
    if (hasConflict) 
        _selected->setConflict(hasConflict);

    return hasConflict;
}

bool SudokuGrid::checkAll()
{
    for (int r = 0; r < SIDE; ++r) {
        for (int c = 0; c < SIDE; ++c) {
            if (isConflicted(r, c))
                return true;
        }
    }
    return false;
}

bool SudokuGrid::checkSelected()
{
    if (_selected == nullptr)
        return false;
    
    return checkCell(_row, _col);
}

bool SudokuGrid::checkCell(const int row, const int col)
{
    GridCell* cell = _cells[row][col];
    const char digit = cell->getDigit();

    if (digit == 0)
        return false;

    bool rowFound = false;
    bool colFound = false;
    for (int i = 0; i < SIDE; ++i) {
        if (!rowFound && i != col && _cells[row][i]->getDigit() == digit) { // Same row
            rowFound = true;
            _cells[row][i]->setConflict(true);
        }
        if (!colFound && i != row && _cells[i][col]->getDigit() == digit) { // Same column
            colFound = true;
            _cells[i][col]->setConflict(true);
        }
    }

    int startRow = (row / GROUP_SIDE) * GROUP_SIDE;
    int startCol = (col / GROUP_SIDE) * GROUP_SIDE;
    bool groupFound = false;
    for (int r = startRow; r < startRow + GROUP_SIDE; ++r) {
        for (int c = startCol; c < startCol + GROUP_SIDE; ++c) {
            if (r == row || c == col)
                continue;

            if (_cells[r][c]->getDigit() == digit) {
                groupFound = true;
                _cells[r][c]->setConflict(true);
                break;
            }
        }

        if (groupFound)
            break;
    }

    return rowFound || colFound || groupFound;
}

void SudokuGrid::refreshConflict(const char digit)
{
    if (_selected == nullptr)
        return;

    for (int i = 0; i < SIDE; ++i) {
        if (i != _col && _cells[_row][i]->getDigit() == digit) { // Same row
            _cells[_row][i]->setConflict(isConflicted(_row, i));
        }
        if (i != _row && _cells[i][_col]->getDigit() == digit) { // Same column
            _cells[_row][i]->setConflict(isConflicted(i, _col));
        }
    }

    int startRow = (_row / GROUP_SIDE) * GROUP_SIDE;
    int startCol = (_col / GROUP_SIDE) * GROUP_SIDE;
    for (int r = startRow; r < startRow + GROUP_SIDE; ++r) {
        for (int c = startCol; c < startCol + GROUP_SIDE; ++c) {
            if (r == _row || c == _col)
                continue;

                if (_cells[r][c]->getDigit() == digit) {
                _cells[r][c]->setConflict(isConflicted(r, c));
            }
        }
    }
}

bool SudokuGrid::isConflicted(const int row, const int col)
{
    GridCell* cell = _cells[row][col];
    const char digit = cell->getDigit();

    if (digit == 0)
        return false;

    for (int i = 0; i < SIDE; ++i) {
        if (i != col && _cells[row][i]->getDigit() == digit) { // Same row
            return true;
        }
        if (i != row && _cells[i][col]->getDigit() == digit) { // Same column
            return true;
        }
    }

    int startRow = (row / GROUP_SIDE) * GROUP_SIDE;
    int startCol = (col / GROUP_SIDE) * GROUP_SIDE;
    for (int r = startRow; r < startRow + GROUP_SIDE; ++r) {
        for (int c = startCol; c < startCol + GROUP_SIDE; ++c) {
            if (r == row || c == col)
                continue;

            if (_cells[r][c]->getDigit() == digit) {
                return true;
            }
        }
    }

    return false;
}
