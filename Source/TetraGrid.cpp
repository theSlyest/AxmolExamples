#include "TetraGrid.h"

USING_NS_AX;

const ax::Color4F TetraGrid::COLOR(0.24f, 0.29f, 0.42f, 1.0f);

const std::map<Tetromino, TetraShape> TetraGrid::SHAPES {
    { Tetromino::I, { { 1, 1, 1, 1 } } },
    { Tetromino::O, { { 1, 1 }, { 1, 1 } } },
    { Tetromino::T, { { 1, 1, 1 }, { 0, 1, 0 } } },
    { Tetromino::S, { { 0, 1, 1 }, { 1, 1, 0 } } },
    { Tetromino::Z, { { 1, 1, 0 }, { 0, 1, 1 } } },
    { Tetromino::L, { { 1, 0, 0 }, { 1, 1, 1 } } },
    { Tetromino::J, { { 0, 0, 1 }, { 1, 1, 1 } } }
};

const std::string TetraGrid::LOCK_EVENT = "BlockLocked";

bool TetraGrid::init()
{
    if ( !DrawNode::init() )
    {
        return false;
    }

    _cells.assign(ROWS, std::vector<TetraCell*>(COLS, nullptr));
    _blockCells.assign(4, nullptr);
    _blockState.assign(4, { 0, 0 });
    _hasBlock = false;
    _period = PERIOD;

    setContentSize(Size(TetraCell::SIZE * COLS + 2.0f * BORDER, TetraCell::SIZE * ROWS + 2.0f * BORDER));
    drawGrid();
    initCellNode();

    scheduleUpdate();

    return true;
}

bool TetraGrid::pushBlock(Tetromino mino)
{
    if (_hasBlock)
        return true;

    _blockShape = SHAPES.at(mino);
    int rows = _blockShape.size();
    int topRow = ROWS - 1;
    int cols = _blockShape[0].size();
    int minCol = (COLS - cols) / 2;
    _blockAxis = { topRow - 1, minCol + 1 };

    int b = 0;
    bool result = true;
    for (int r = 0; r < rows; ++r) {
        const int row = topRow - r;
        const float y = row * TetraCell::SIZE;
        for (int c = 0; c < cols; ++c) {
            if (!_blockShape[r][c])
                continue;
            
            int col = minCol + c;
            auto cell = TetraCell::create(mino);
            cell->setPosition(col * TetraCell::SIZE, y);
            _cellNode->addChild(cell);

            if (_cells[row][col] != nullptr)
                result = false;
            
            _blockCells[b] = cell;
            _blockState[b++] = { row, col };
        }
    }

    _elapsed = 0.0f;
    _hasBlock = result;
    _blockMino = mino;

    if (!result)
        unscheduleUpdate();

    return result;
}

void TetraGrid::update(float dt)
{
    _elapsed += dt;
    if (_elapsed >= _period) {
        _elapsed = 0.0f;
        if (!stepBlock()) {
            lockBlock();
        }
    }
}

void TetraGrid::shiftBlock(bool right)
{
    if (!_hasBlock)
        return;

    int factor = right ? 1 : -1;
    std::vector<GridPos> newState;
    for (int i = 0; i < 4; ++i) {
        const int col = _blockState[i].second + factor;
        if (col == -1 || col == COLS || _cells[_blockState[i].first][col] != nullptr)
            return;

        newState.push_back({ _blockState[i].first, col });
    }

    for (int i = 0; i < 4; ++i) {
        _blockState[i] = newState[i];
        _blockCells[i]->setPositionX(_blockCells[i]->getPositionX() + factor * TetraCell::SIZE);
    }

    _blockAxis.second += factor;
}

void TetraGrid::speedUpBlock(bool up)
{
    _period = up ? PERIOD / 4.0f : PERIOD;
}

void TetraGrid::turnBlock()
{
    if (!_hasBlock)
        return;

    std::vector<GridPos> newState;
    switch (_blockMino) {
        case Tetromino::O:
            return;
        
        case  Tetromino::I:
            if (!turnI(newState))
                return;
            break;
        
        default:
            if (!turnDefault(newState))
                return;
            break;
    }

    for (int i = 0; i < 4; ++i) {
        _blockState[i] = newState[i];
        _blockCells[i]->setPosition(newState[i].second * TetraCell::SIZE, newState[i].first * TetraCell::SIZE);
    }
}

void TetraGrid::dropBlock()
{
    if (!_hasBlock)
        return;

    bool stepped = true;
    while (stepped) {
        std::vector<GridPos> state;
        for (int i = 0; i < 4; ++i) {
            const GridPos pos = { _blockState[i].first - 1, _blockState[i].second };

            if (pos.first < 0 || _cells[pos.first][pos.second] != nullptr) {
                stepped = false;
                break;
            }

            state.push_back(pos);
        }

        if (stepped)
            _blockState = state;
    }

    for (int i =0; i < 4;  ++i) {
        _blockCells[i]->setPositionY(_blockState[i].first * TetraCell::SIZE);
    }

    lockBlock();
}

void TetraGrid::reset()
{
    _cellNode->removeAllChildren();
    _hasBlock = false;
    _cells.assign(ROWS, std::vector<TetraCell*>(COLS, nullptr));
    _blockState.assign(4, { 0, 0 });
    _blockCells.assign(4, nullptr);
    _elapsed = 0.0f;
    scheduleUpdate();
}

void TetraGrid::drawGrid()
{
    float endX = _contentSize.width - BORDER;
    float endY = _contentSize.height - BORDER;

    for (int c = 1; c < COLS; ++c) {
        const float x = BORDER + c * TetraCell::SIZE;
        drawLine(Vec2(x, BORDER), Vec2(x, endY), COLOR, LINE);
    }

    for (int r = 1; r < ROWS; ++r) {
        const float y = BORDER + r * TetraCell::SIZE;
        drawLine(Vec2(BORDER, y), Vec2(endX, y), COLOR, LINE);
    }

    float bOffset = BORDER / 2.0f;
    drawRect(Vec2(bOffset, bOffset), Vec2(_contentSize.width - 2.0f, _contentSize.height - bOffset), COLOR, LINE);
}

void TetraGrid::initCellNode()
{
    _cellNode = Node::create();
    _cellNode->setContentSize(Size(TetraCell::SIZE * COLS, TetraCell::SIZE * ROWS));
    _cellNode->setPosition(BORDER, BORDER);
    addChild(_cellNode);
}

bool TetraGrid::stepBlock()
{
    if (!_hasBlock)
        return true;

    std::vector<GridPos> newState;
    for (int i = 0; i < 4; ++i) {
        const GridPos pos = { _blockState[i].first - 1, _blockState[i].second };
        if (pos.first < 0 || _cells[pos.first][pos.second] != nullptr)
            return false;
        
        newState.push_back(pos);
    }

    for (int i = 0; i < 4; ++i) {
        _blockState[i] = newState[i];
        _blockCells[i]->setPositionY(_blockCells[i]->getPositionY() - TetraCell::SIZE);
    }

    _blockAxis.first -= 1;

    return true;
}

bool TetraGrid::turnI(std::vector<GridPos> &state)
{
    if (_blockState[0].first == _blockState[1].first) { // Horizontal to vertical
        const int rowStart = std::min(_blockState[0].first + 1, ROWS - 1);
        const int col = _blockState[1].second;
        for (int r = 0; r < 4; ++r) {
            const int row = rowStart - r;
            if (row < 0 || _cells[row][col] != nullptr)
                return false;
            
            state.push_back({ row, col });
        }
    }
    else { // Vertical to horizontal
        const int row = _blockState[1].first;
        int colStart = std::min(_blockState[1].second - 1, 0);
        colStart = std::min(colStart, COLS - 4);
        for (int c = 0; c < 4; ++c) {
            const int col = colStart + c;
            if (_cells[row][col] != nullptr)
                return false;

            state.push_back({ row, col });
        }
    }

    return true;
}

bool TetraGrid::turnDefault(std::vector<GridPos> &state)
{
    TetraShape rotated;
    const int rows = _blockShape.size();
    const int cols = _blockShape[0].size();
    int shift = 0;

    for (int i = 0; i < 4; ++i) {
        int row = _blockAxis.first + _blockState[i].second - _blockAxis.second;
        int col = _blockAxis.second - _blockState[i].first + _blockAxis.first;

        if (row < 0)
            return false;

        if (col < 0 || col > COLS - 1)
            shift = col < 0 ? std::max(shift, -col) : std::min(shift, COLS - 1 - col);

        state.push_back({ row, col });
    }

    for (int i = 0; i < 4; ++i) {
        if (shift != 0)
            state[i].second += shift;

        if (_cells[state[i].first][state[i].second] != nullptr)
            return false;
    }

    return true;
}

void TetraGrid::lockBlock()
{
    _hasBlock = false;
    for (int i = 0; i < 4; ++i) {
        _cells[_blockState[i].first][_blockState[i].second] = _blockCells[i];
        _blockState[i] = { 0, 0 };
        _blockCells[i] = nullptr;
    }

    const int lines = clearLines();
    _eventDispatcher->dispatchCustomEvent(LOCK_EVENT, new LinesData(lines));
}

int TetraGrid::clearLines()
{
    bool isEmpty = false;
    int r = 0;
    int cleared = 0;
    while (!isEmpty && r < ROWS) {
        isEmpty = true;
        bool isLine = true;

        for (int c = 0; c < COLS; ++c) {
            if (!isEmpty && !isLine)
                break;

            if (_cells[r][c] == nullptr)
                isLine = false;
            else
                isEmpty = false;
        }

        if (isLine) {
            ++cleared;
            int ri = r + 1;
            bool hasCells = true;

            for (int c = 0; c < COLS; ++c) {
                _cells[r][c]->setVisible(false);
                _cellNode->removeChild(_cells[r][c]);
            }

            while (ri < ROWS && hasCells) {
                hasCells = false;
                for (int c = 0; c < COLS; ++c) {
                    auto cell = _cells[ri][c];
                    if (cell != nullptr) {
                        hasCells = true;
                        cell->setPositionY(cell->getPositionY() - TetraCell::SIZE);
                    }

                    _cells[ri - 1][c] = cell;
                }
                ++ri;
            }
        }
        else
            ++r;
    }

    return cleared;
}
