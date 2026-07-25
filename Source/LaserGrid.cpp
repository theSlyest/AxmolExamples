#include "LaserGrid.h"

#include "Mirror.h"
#include "Target.h"
#include "Wall.h"

USING_NS_AX;

const Color4F LaserGrid::BORDER_COLOR(0.17f, 0.25f, 0.31f, 1.0f);

const std::map<LaserSide, std::pair<int, int>> LaserGrid::SHIFTS {
    { LaserSide::LEFT,   {  0, -1 } },
    { LaserSide::RIGHT,  {  0,  1 } },
    { LaserSide::BOTTOM, { -1,  0 } },
    { LaserSide::TOP,    {  1,  0 } }
};

bool LaserGrid::init()
{
    if (!DrawNode::init())
    {
        return false;
    }

    Size size(2.0f * BORDER + GridCell::SIZE * COLS, 2.0f * BORDER + GridCell::SIZE * ROWS);
    setContentSize(size);

    float borderOffset = BORDER / 2.0f;
    drawRect(Vec2(borderOffset, borderOffset), Vec2(size.width - borderOffset, size.height - borderOffset), BORDER_COLOR, BORDER);

    initCells();
    initEmitter();

    _emitterSide = LaserSide::LEFT;
    _emitterRow = 0;
    _emitterCol = 0;

    return true;
}

void LaserGrid::initCells()
{
    for (int r = 0; r < ROWS; ++r) {
        _cells.push_back(std::vector<GridCell *>(COLS, nullptr));
    }
}

void LaserGrid::initEmitter()
{
    _emitter = Emitter::create();
    addChild(_emitter);
}

LaserSide LaserGrid::invertSide(LaserSide side) const
{
    return (LaserSide)(((int)side + 2) % (int)4);
}

void LaserGrid::positionEmitter(LaserSide side, int position)
{
    _emitterSide = side;

    float x;
    float y;
    float angle;
    const float padding = (GridCell::SIZE - Emitter::SIZE.height) / 2.0f;

    switch (side)
    {
    case LaserSide::LEFT:
        _emitterRow = position;
        _emitterCol = 0;
        x = BORDER - Emitter::SIZE.width;
        y = BORDER + position * GridCell::SIZE + padding;
        angle = 0.0f;
        break;

    case LaserSide::RIGHT:
        _emitterRow = position;
        _emitterCol = COLS - 1;
        x = _contentSize.width - BORDER;
        y = BORDER + position * GridCell::SIZE + padding;
        angle = 180.0f;
        break;
    case LaserSide::BOTTOM:
        _emitterRow = 0;
        _emitterCol = position;
        x = BORDER + position * GridCell::SIZE + padding;
        y = BORDER - Emitter::SIZE.width;
        angle = 90.0f;
        break;

    case LaserSide::TOP:
        _emitterRow = ROWS - 1;
        _emitterCol = position;
        x = BORDER + position * GridCell::SIZE + padding;
        y = _contentSize.height - BORDER;
        angle = 270.0f;
        break;
    }

    _cells[_emitterRow][_emitterCol]->setTag(1);
    _emitter->setRotation(angle);
    _emitter->setPosition(x, y);
}

void LaserGrid::reset(const std::vector<std::vector<CellState>> &states)
{
    for (int r = 0; r < ROWS; ++r) {
        for (int c = 0; c < COLS; ++c) {

            if (_cells[r][c] != nullptr) {
                removeChild(_cells[r][c], true);
            }

            CellState state = states[r][c];
            if (state == CellState::EMPTY) {
                _cells[r][c] = Mirror::create();
            }
            else if (state == CellState::WALL) {
                _cells[r][c] = Wall::create();
            }
            else {
                _cells[r][c] = Target::create();
            }

            _cells[r][c]->setPosition(BORDER + c * GridCell::SIZE, BORDER + r * GridCell::SIZE);
            addChild(_cells[r][c]);
        }
    }

    _mirrorCount = 0;
}

void LaserGrid::clearBeam()
{
    for (auto& row : _cells) {
        for (GridCell* cell : row) {
            if (cell->isLit())
                cell->laserOff();
        }
    }
}

bool LaserGrid::refreshBeam()
{
    clearBeam();

    int row;
    int col;
    if (_emitterSide == LaserSide::LEFT || _emitterSide == LaserSide::RIGHT) {
        row = _emitterRow;
        col = _emitterSide == LaserSide::LEFT ? -1 : COLS;
    }
    else {
        row = _emitterSide == LaserSide::BOTTOM ? -1 : ROWS;
        col = _emitterCol;
    }

    LaserSide from;
    LaserSide to = invertSide(_emitterSide);
    do {
        auto shift = SHIFTS.at(to);
        row += shift.first;
        col += shift.second;
        from = invertSide(to);
    } while (row > -1 && row < ROWS && col > -1 && col < COLS && _cells[row][col]->laserOn(from, to));

    if (row > -1 && row < ROWS && col > -1 && col < COLS) {
        return _cells[row][col]->getState() == CellState::TARGET;
    }

    return false;
}

bool LaserGrid::onTouch(Touch *touch, bool maxReached)
{
    const Vec2& touchPos = convertTouchToNodeSpace(touch);
    for (auto& row : _cells) {
        for (GridCell* cell : row) {
            if (cell->getBoundingBox().containsPoint(touchPos)) {

                if (cell->getTag() == 1)
                    return true;

                    CellState state = cell->getState();
                if (state == CellState::EMPTY || state == CellState::MIRROR) {

                    if (state == CellState::EMPTY && maxReached)
                        return true;

                    cell->onTouch();

                    if (state != cell->getState()) {
                        if (state == CellState::EMPTY)
                            ++_mirrorCount;
                        else
                            --_mirrorCount;
                    }

                    if (cell->isLit())
                        _isTargetReached = refreshBeam();
                }
                return true;
            }
        }
    }

    return false;
}
