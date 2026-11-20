#include "LaserGrid.h"

#include "Target.h"
#include "Wall.h"
#include "base/Touch.h"

USING_NS_AX;

const Color4F LaserGrid::BORDER_COLOR(0.17f, 0.25f, 0.31f, 1.0f);

LaserGrid* LaserGrid::create(const LaserBoard& board)
{
    if (auto* pRet = new (std::nothrow) LaserGrid(); pRet && pRet->init(board))
    {
        pRet->autorelease();
        return pRet;
    }
    else
    {
        delete pRet;
        pRet = nullptr;
        return nullptr;
    }
}

bool LaserGrid::init(const LaserBoard& board)
{
    if (!DrawNode::init())
        return false;

    _board = &board;

    constexpr Vec2 size(2.0f * BORDER + GridCell::SIZE * COLS, 2.0f * BORDER + GridCell::SIZE * ROWS);
    setContentSize(size);

    constexpr float borderOffset = BORDER / 2.0f;
    drawRect(Vec2(borderOffset, borderOffset), Vec2(size.width - borderOffset, size.height - borderOffset), BORDER_COLOR, BORDER);

    initCells();
    initEmitter();
    refresh();

    return true;
}

void LaserGrid::initCells()
{
    for (int row = 0; row < ROWS; ++row) {
        _cells.emplace_back(COLS, nullptr);
        _mirrors.emplace_back(COLS, nullptr);

        for (int col = 0; col < COLS; ++col) {
            GridCell* cell;
            switch (_board->getState(row, col)) {
                case CellState::WALL:
                    cell = Wall::create();
                    break;

                case CellState::TARGET:
                    cell = Target::create();
                    break;

                default:
                    _mirrors[row][col] = Mirror::create();
                    cell = _mirrors[row][col];
                    break;
            }

            cell->setPosition(BORDER + col * GridCell::SIZE, BORDER + row * GridCell::SIZE);
            addChild(cell);
            _cells[row][col] = cell;
        }
    }
}

void LaserGrid::initEmitter()
{
    _emitter = Emitter::create();
    addChild(_emitter);

    // The emitter sits outside the grid, in front of its row or column, pointing in.
    const int position = _board->getEmitterPos();
    const float padding = (GridCell::SIZE - Emitter::SIZE.height) / 2.0f;
    float x = 0.0f;
    float y = 0.0f;
    float angle = 0.0f;

    switch (_board->getEmitterSide()) {
        case LaserSide::LEFT:
            x = BORDER - Emitter::SIZE.width;
            y = BORDER + position * GridCell::SIZE + padding;
            angle = 0.0f;
            break;

        case LaserSide::RIGHT:
            x = _contentSize.width - BORDER;
            y = BORDER + position * GridCell::SIZE + padding;
            angle = 180.0f;
            break;

        case LaserSide::BOTTOM:
            x = BORDER + position * GridCell::SIZE + padding;
            y = BORDER - Emitter::SIZE.width;
            angle = 90.0f;
            break;

        case LaserSide::TOP:
            x = BORDER + position * GridCell::SIZE + padding;
            y = _contentSize.height - BORDER;
            angle = 270.0f;
            break;
    }

    _emitter->setRotation(angle);
    _emitter->setPosition(x, y);
}

void LaserGrid::refresh() const
{
    for (const auto& row : _cells) {
        for (GridCell* cell : row)
            cell->clearBeam();
    }

    for (int row = 0; row < ROWS; ++row) {
        for (int col = 0; col < COLS; ++col) {
            if (Mirror* mirror = _mirrors[row][col])
                mirror->setMirror(_board->getState(row, col) == CellState::MIRROR, _board->isBackslash(row, col));
        }
    }

    for (const auto& step : _board->getBeam().steps)
        _cells[step.row][step.col]->showBeam(step.from, step.to);
}

std::optional<std::pair<int, int>> LaserGrid::cellAt(Touch* touch) const
{
    const Vec2 position = convertTouchToNodeSpace(touch) - Vec2(BORDER, BORDER);
    if (position.x < 0.0f || position.y < 0.0f)
        return std::nullopt;

    const int col = static_cast<int>(position.x / GridCell::SIZE);
    const int row = static_cast<int>(position.y / GridCell::SIZE);
    if (row >= ROWS || col >= COLS)
        return std::nullopt;

    return std::make_pair(row, col);
}
