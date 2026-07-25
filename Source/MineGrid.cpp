#include "MineGrid.h"

USING_NS_AX;

const Color4F MineGrid::BORDER_COLOR(0.22f, 0.3f, 0.42f, 1.0f);

bool MineGrid::init()
{
    if (!DrawNode::init())
    {
        return false;
    }

    _coveredCount = SIZE * SIZE;
    _isMineRevealed = false;

    float side = OFFSET * 2.0f + SIZE * (MineCell::SIZE + GAP) - GAP;
    Size size(side, side);
    setContentSize(size);

    float bOffset = BORDER / 2.0f;
    drawRect(Vec2(bOffset, bOffset), Vec2(side - bOffset, side - bOffset), BORDER_COLOR, BORDER);

    initCells();

    return true;
}

void MineGrid::initCells()
{
    float shift = MineCell::SIZE + GAP;
    for (int r = 0; r < SIZE; ++r) {
        const float y = OFFSET + shift * r;
        std::vector<MineCell*> row;
        for (int c = 0; c < SIZE; ++c) {
            auto cell = MineCell::create();
            cell->setPosition(OFFSET + shift * c, y);
            addChild(cell);
            row.push_back(cell);
        }
        _cells.push_back(row);
    }
}

void MineGrid::revealMines()
{
    for (const auto& coords : _mineCoords) {
        MineCell* cell = _cells[coords.first][coords.second];
        if (cell->isFlagged())
            continue;
        
        _cells[coords.first][coords.second]->setCovered(false);
    }

    for (const auto& coords : _flagCoords) {
        MineCell* cell = _cells[coords.first][coords.second];
        if (cell->isMine())
            continue;
        
        _cells[coords.first][coords.second]->setCrossed(true);
    }
}

void MineGrid::revealAdjacent(int row, int col)
{
    if (_cells[row][col]->getMineCount() > 0)
        return;

    const int minRow = std::max(row - 1, 0);
    const int maxRow = std::min(row + 2, SIZE);
    const int minCol = std::max(col - 1, 0);
    const int maxCol = std::min(col + 2, SIZE);

    int count = 0;
    for (int r = minRow; r < maxRow; ++r) {
        for (int c = minCol; c < maxCol; ++c) {
            if (r == row && c == col)
                continue;
            
            MineCell* cell = _cells[r][c];
            if (cell->isMine() || !cell->isCovered())
                continue;
            
            cell->setCovered(false);
            --_coveredCount;
            if (cell->getMineCount() == 0)
                revealAdjacent(r, c);
        }
    }
}

void MineGrid::countMines(int row, int col)
{
    if (_cells[row][col]->isMine())
        return;

    const int minRow = std::max(row - 1, 0);
    const int maxRow = std::min(row + 2, SIZE);
    const int minCol = std::max(col - 1, 0);
    const int maxCol = std::min(col + 2, SIZE);

    int count = 0;
    for (int r = minRow; r < maxRow; ++r) {
        for (int c = minCol; c < maxCol; ++c) {
            if (r == row && c == col)
                continue;
            
            if (_cells[r][c]->isMine()) 
                ++count;
        }
    }
    _cells[row][col]->setMineCount(count);
}

MineCell *MineGrid::locateCell(const Vec2 &location)
{
    const Vec2& point = convertToNodeSpace(location);

    for (const auto& row : _cells) {
        for (MineCell* cell : row) {
            if (cell->getBoundingBox().containsPoint(point)) 
                return cell;
        }
    }

    return nullptr;
}

std::pair<int, int> MineGrid::locateCoords(const Vec2 &location)
{
    const Vec2& point = convertToNodeSpace(location);
    float shift = MineCell::SIZE + GAP;
    std::pair<int, int> coords { int((point.y - OFFSET) / shift), int((point.x - OFFSET) / shift) };

    if (coords.first < 0 || coords.first >= SIZE || coords.second < 0 || coords.second >= SIZE)
        return { -1,  -1 };

    return coords;
}

void MineGrid::hover(const Vec2 &location)
{
    MineCell* cell = locateCell(location);

    if (cell == _hovered)
        return;

    if (cell == nullptr) {
        _hovered->setHovered(false);
        _hovered = nullptr;
        return;
    }

    if (_hovered != nullptr)
        _hovered->setHovered(false);

    cell->setHovered(true);
    _hovered = cell;
    return;
}

bool MineGrid::reveal(const Vec2 &location)
{
    MineCell* cell = locateCell(location);

    if (cell == nullptr)
        return false;
    
    if (!cell->isCovered() || cell->isFlagged())
        return false;

    cell->setCovered(false);
    --_coveredCount;
    
    if (cell->isMine()) {
        _isMineRevealed = true;
        revealMines();
    }
    else {
        const std::pair<int, int>& coords = locateCoords(location);
        revealAdjacent(coords.first, coords.second);
    }

    return true;
}

bool MineGrid::switchFlag(const Vec2 &location)
{
    const std::pair<int, int>& coords = locateCoords(location);

    if (coords.first == -1)
        return false;

    MineCell* cell = _cells[coords.first][coords.second];

    bool oldFlagged = cell->isFlagged();
    cell->setFlagged(!oldFlagged);
    
    if (oldFlagged == cell->isFlagged())
        return false;
    
    if (oldFlagged)
        _flagCoords.erase(coords);
    else
        _flagCoords.insert(coords);

    return true;
}

bool MineGrid::populateFrom(const Vec2 &location, unsigned int mines)
{
    const std::pair<int, int>& coords = locateCoords(location);
    if (coords.first == -1)
        return false;
    
    populateExcept(coords, mines);
    return true;
}

void MineGrid::populateExcept(const std::pair<int, int>& excluded, unsigned int mines)
{
    _mineCoords.insert(excluded);
    while (_mineCoords.size() < mines + 1u) {
        const std::pair<int, int> coords = { random(0, SIZE - 1), random(0, SIZE - 1) };
        if (_mineCoords.insert(coords).second) {
            _cells[coords.first][coords.second]->setMine(true);
        }
    }
    _mineCoords.erase(excluded);

    for (int r = 0; r < SIZE; ++r) {
        for (int c = 0; c < SIZE; ++c) {
            countMines(r, c);
        }
    }
}

void MineGrid::reset()
{
    for (const auto& row : _cells) {
        for (MineCell* cell : row) {
            cell->setMine(false);
            cell->setMineCount(0);
            cell->setFlagged(false);
            cell->setCovered(true);
            cell->setCrossed(false);
        }
    }

    _mineCoords.clear();
    _flagCoords.clear();
    _coveredCount = SIZE * SIZE;
    _isMineRevealed = false;
}
