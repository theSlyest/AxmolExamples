#include "PegBoard.h"

USING_NS_AX;

bool PegBoard::init()
{
    if ( !DrawNode::init() )
    {
        return false;
    }

    setContentSize(Size(SIZE, SIZE));

    const Vec2 points[8] = {
        Vec2(RADIUS, 0.0f),
        Vec2(SIZE - RADIUS, 0.0f),
        Vec2(SIZE, RADIUS),
        Vec2(SIZE, SIZE - RADIUS),
        Vec2(SIZE - RADIUS, SIZE),
        Vec2(RADIUS, SIZE),
        Vec2(0.0f, SIZE - RADIUS),
        Vec2(0.0f, RADIUS)
    };

    drawSolidPoly(points, 8, COLOR);

    const int segments = RADIUS * M_PI * 2.0f;
    drawSolidCircle(Vec2(RADIUS, RADIUS), RADIUS, 0.0f, segments, COLOR);
    drawSolidCircle(Vec2(SIZE - RADIUS, RADIUS), RADIUS, 0.0f, segments, COLOR);
    drawSolidCircle(Vec2(SIZE - RADIUS, SIZE - RADIUS), RADIUS, 0.0f, segments, COLOR);
    drawSolidCircle(Vec2(RADIUS, SIZE - RADIUS), RADIUS, 0.0f, segments, COLOR);

    initCells();

    return true;
}

void PegBoard::reset() const
{
    for (int c = 0; c < LENGTH; ++c) {
        for (int r = 0; r < LENGTH; ++r) {
            if (_cells[c][r] == nullptr)
                continue;
        
            _cells[c][r]->setPeg(c != 3 || r != 3);
            _cells[c][r]->setSelected(false);
        }
    }
}

PegCoords PegBoard::onTouch(ax::Touch *touch) const
{
    const Vec2& point = convertTouchToNodeSpace(touch);

    for (int c = 0; c < LENGTH; ++c) {
        for (int r = 0; r < LENGTH; ++r) {
            if (_cells[c][r] == nullptr)
                continue;
        
            if (_cells[c][r]->getBoundingBox().containsPoint(point))
                return {c, r};
        }
    }

    return {-1, -1};
}

bool PegBoard::hasJumps()
{
    for (int c = 0; c < LENGTH; ++c) {
        for (int r = 0; r < LENGTH; ++r) {
            if (_cells[c][r] == nullptr)
                continue;
        
            if (canJump(c, r))
                return true;
        }
    }

    return false;
}

void PegBoard::initCells()
{
    for (int c = 0; c < LENGTH; ++c) {
        std::vector<PegCell*> col;
        for (int r = 0; r < LENGTH; ++r) {
            if ((c < 2 || c > 4) && (r < 2 || r > 4)) {
                col.push_back(nullptr);
                continue;
            }
        
            auto cell = PegCell::create(c != 3 || r != 3);
            cell->setPosition(OFFSET + c * PegCell::SIZE, OFFSET + r * PegCell::SIZE);
            addChild(cell);
            col.push_back(cell);
        }
        _cells.push_back(col);
    }
}

bool PegBoard::canJump(const int col, const int row) const
{
    if (!_cells[col][row]->hasPeg())
        return false;
    
    if (col > 1 && _cells[col - 2][row] != nullptr && _cells[col - 1][row]->hasPeg() && !_cells[col - 2][row]->hasPeg())
        return true;
    
    if (col < LENGTH - 2 && _cells[col + 2][row] != nullptr && _cells[col + 1][row]->hasPeg() && !_cells[col + 2][row]->hasPeg())
        return true;
    
    if (row > 1 && _cells[col][row - 2] != nullptr && _cells[col][row - 1]->hasPeg() && !_cells[col][row - 2]->hasPeg())
        return true;
    
    if (row < LENGTH - 2 && _cells[col][row + 2] != nullptr && _cells[col][row + 1]->hasPeg() && !_cells[col][row + 2]->hasPeg())
        return true;
    
    return false;
}
