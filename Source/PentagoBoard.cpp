#include "PentagoBoard.h"

#include <ranges>

USING_NS_AX;

bool PentagoBoard::init()
{
    if (!Node::init())
        return false;

    constexpr float size = 2.0f * PentagoBlock::SIZE + SPACING;
    setContentSize(Vec2(size, size));

    constexpr float shift = PentagoBlock::SIZE + SPACING;
    for (int c = 0; c < 2; ++c) {
        std::vector<PentagoBlock*> col;
        const float x = c * shift;
        for (int r = 0; r < 2; ++r) {
            auto block = PentagoBlock::create(r == 1);
            block->setPosition(x, r * shift );
            addChild(block);
            col.push_back(block);
        }
        _blocks.push_back(col);
    }
    
    return true;
}

void PentagoBoard::reset()
{
    for (auto& col : _blocks) {
        for (const auto block : col) {
            block->reset();
        }
    }
    _lines.clear();
}

bool PentagoBoard::onTouch(Touch *touch, bool isBlack) const
{
    if (!PentagoBlock::isEnabled() || PentagoBlock::isTurning())
        return false;
    
    const Vec2& location = convertTouchToNodeSpace(touch);

    for (auto& col : _blocks) {
        for (const auto block : col) {
            if (block->getBoundingBox().containsPoint(location))
                return block->onTouch(touch->getLocation(), isBlack);
        }
    }

    return false;
}

bool PentagoBoard::checkLines() 
{
    const bool hasVertical = checkVertical();
    const bool hasHorizontal = checkHorizontal();
    const bool hasDiagonal = checkDiagonal();

    return hasVertical || hasDiagonal || hasHorizontal;
}

void PentagoBoard::ringLines() const
{
    for (const auto& val : _lines | std::views::values) {
        for (const auto socket : val)
            socket->setRing(true);
    }
}

PentagoSocket *PentagoBoard::get(const int col, const int row) const
{
    if (col < 0 || col > 5 || row < 0 || row > 5)
        return nullptr;

    const int blockCol = col / 3;
    const int blockRow = row / 3;
    const int socketCol = col % 3;
    const int socketRow = row % 3;
    return _blocks[blockCol][blockRow]->at(socketCol)[socketRow];
}

bool PentagoBoard::checkHorizontal()
{
    bool result = false;

    for (int r = 0; r < 6; ++r) {
        if (get(1, r)->isEmpty() || get(4, r)->isEmpty() || get(1, r)->isBlack() != get(4, r)->isBlack())
            continue;

        bool isLine = true;
        const int start = (get(0, r)->isEmpty() || get(0, r)->isBlack() != get(1, r)->isBlack()) ? 1 : 0;
        const bool isBlack = get(start, r)->isBlack();
        const int limit = start + 5;
        for (int c = 2; c < limit ; ++c) {
            if (get(c, r)->isEmpty() || get(c, r)->isBlack() != isBlack) {
                isLine = false;
                break;
            }
        }

        if (isLine) {
            result = true;
            for (int c = start; c < limit ; ++c)
                _lines[isBlack].insert(get(c, r));
        }
    }

    return result;
}

bool PentagoBoard::checkVertical()
{
    bool result = false;

    for (int c = 0; c < 6; ++c) {
        if (get(c, 1)->isEmpty() || get(c, 4)->isEmpty() || get(c, 1)->isBlack() != get(c, 4)->isBlack())
            continue;

        bool isLine = true;
        const int start = (get(c, 0)->isEmpty() || get(c, 0)->isBlack() != get(c, 1)->isBlack()) ? 1 : 0;
        const bool isBlack = get(c, start)->isBlack();
        const int limit = start + 5;
        for (int r = 2; r < limit ; ++r) {
            if (get(c, r)->isEmpty() || get(c, r)->isBlack() != isBlack) {
                isLine = false;
                break;
            }
        }

        if (isLine) {
            result = true;
            for (int r = start; r < limit ; ++r) 
                _lines[isBlack].insert(get(c, r));
        }
    }

    return result;
}

bool PentagoBoard::checkDiagonal()
{
    const bool diagonal5_0 = checkDiagonalOf5(0);
    const bool diagonal5_1 = checkDiagonalOf5(1);
    const bool diagonal5_4 = checkDiagonalOf5(4);
    const bool diagonal5_5 = checkDiagonalOf5(5);
    const bool diagonal6_false = checkDiagonalOf6(false);
    const bool diagonal6_true = checkDiagonalOf6(true);

    return diagonal5_0 || diagonal5_1 || diagonal5_4 || diagonal5_5 || diagonal6_false || diagonal6_true;
}

bool PentagoBoard::checkDiagonalOf5(const int startRow)
{
    if (startRow == 2 || startRow == 3)
        return false;

    const int startCol = (startRow == 0 || startRow == 5) ? 1 : 0;
    if (get(startCol, startRow)->isEmpty())
        return false;

    const bool isBlack = get(startCol, startRow)->isBlack();
    for (int i = 1; i < 5; ++i) {
        const int row = (startRow > 2) ? startRow - i : startRow + i;
        if (get(startCol + i, row)->isEmpty() || get(startCol + i, row)->isBlack() != isBlack)
            return false;
    }

    LinedSockets result;

    for (int i = 0; i < 5; ++i) {
        const int row = (startRow > 2) ? startRow - i : startRow + i;
        _lines[isBlack].insert(get(startCol + i, row));
    }

    return true;
}

bool PentagoBoard::checkDiagonalOf6(bool lastRow)
{
    int startRow;
    int rowStep;
    if (lastRow) {
        startRow = 5;
        rowStep = -1;
    }
    else {
        startRow = 0;
        rowStep = 1;
    }

    if (get(1, startRow + rowStep)->isEmpty() 
        || get(4, startRow + 4 * rowStep)->isEmpty() 
        || get(1, startRow + rowStep)->isBlack() != get(4, startRow + 4 * rowStep)->isBlack())
        return false;
    
    const int startCol = (get(0, startRow)->isEmpty() || get(0, startRow)->isBlack() != get(1, startRow + rowStep)->isBlack()) ? 1 : 0;
    const bool isBlack = get(startCol, startRow)->isBlack();
    const int limit = startCol + 5;
    for (int i = 2; i < limit; ++i) {
        const int col = startRow + i * rowStep;
        if (get(i, col)->isEmpty() || get(i, col)->isBlack() != isBlack)
            return false;
    }

    LinedSockets result;

    for (int i = startCol; i < limit; ++i) {
        _lines[isBlack].insert(get(i, startRow + i * rowStep));
    }

    return true;
}
