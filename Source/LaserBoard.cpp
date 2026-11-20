#include "LaserBoard.h"

#include <cassert>
#include <utility>

namespace {
// Where the beam goes next when it leaves a cell through the given side.
std::pair<int, int> shiftOf(const LaserSide side)
{
    switch (side) {
        case LaserSide::LEFT:   return { 0, -1 };
        case LaserSide::RIGHT:  return { 0, 1 };
        case LaserSide::BOTTOM: return { -1, 0 };
        case LaserSide::TOP:    return { 1, 0 };
    }

    return { 0, 0 };
}
}

LaserSide LaserBoard::opposite(const LaserSide side)
{
    switch (side) {
        case LaserSide::LEFT:   return LaserSide::RIGHT;
        case LaserSide::RIGHT:  return LaserSide::LEFT;
        case LaserSide::BOTTOM: return LaserSide::TOP;
        case LaserSide::TOP:    return LaserSide::BOTTOM;
    }

    return side;
}

LaserBoard::LaserBoard(const std::vector<std::string>& layout, const LaserSide emitterSide, const int emitterPos,
                       const int maxMirrors)
    : _cells(ROWS, std::vector<Cell>(COLS)), _emitterSide(emitterSide), _emitterPos(emitterPos), _maxMirrors(maxMirrors)
{
    assert(static_cast<int>(layout.size()) == ROWS);

    for (int row = 0; row < ROWS; ++row) {
        // The layout is written from the top row down.
        const std::string& line = layout[ROWS - 1 - row];
        assert(static_cast<int>(line.size()) == COLS);

        for (int col = 0; col < COLS; ++col) {
            if (line[col] == '#')
                _cells[row][col].state = CellState::WALL;
            else if (line[col] == 'T')
                _cells[row][col].state = CellState::TARGET;
        }
    }

    traceBeam();
}

void LaserBoard::reset()
{
    for (auto& row : _cells) {
        for (auto& cell : row) {
            if (cell.state == CellState::MIRROR)
                cell.state = CellState::EMPTY;

            cell.backslash = false;
        }
    }

    _mirrorCount = 0;
    traceBeam();
}

bool LaserBoard::isEmitterCell(const int row, const int col) const
{
    switch (_emitterSide) {
        case LaserSide::LEFT:   return row == _emitterPos && col == 0;
        case LaserSide::RIGHT:  return row == _emitterPos && col == COLS - 1;
        case LaserSide::BOTTOM: return row == 0 && col == _emitterPos;
        case LaserSide::TOP:    return row == ROWS - 1 && col == _emitterPos;
    }

    return false;
}

bool LaserBoard::cycleMirror(const int row, const int col)
{
    if (row < 0 || row >= ROWS || col < 0 || col >= COLS || isEmitterCell(row, col))
        return false;

    Cell& cell = _cells[row][col];
    if (cell.state == CellState::EMPTY) {
        if (_mirrorCount >= _maxMirrors)
            return false;

        cell.state = CellState::MIRROR;
        cell.backslash = false;
        ++_mirrorCount;
    }
    else if (cell.state == CellState::MIRROR) {
        if (cell.backslash) {
            cell.state = CellState::EMPTY;
            cell.backslash = false;
            --_mirrorCount;
        }
        else
            cell.backslash = true;
    }
    else
        return false;

    traceBeam();
    return true;
}

LaserSide LaserBoard::reflect(const Cell& mirror, const LaserSide from) const
{
    if (mirror.backslash) {
        switch (from) {
            case LaserSide::LEFT:   return LaserSide::BOTTOM;
            case LaserSide::BOTTOM: return LaserSide::LEFT;
            case LaserSide::RIGHT:  return LaserSide::TOP;
            case LaserSide::TOP:    return LaserSide::RIGHT;
        }
    }

    switch (from) {
        case LaserSide::LEFT:   return LaserSide::TOP;
        case LaserSide::TOP:    return LaserSide::LEFT;
        case LaserSide::RIGHT:  return LaserSide::BOTTOM;
        case LaserSide::BOTTOM: return LaserSide::RIGHT;
    }

    return from;
}

void LaserBoard::traceBeam()
{
    _beam = Beam{};

    // Start just outside the grid, in front of the emitter, heading into it.
    int row;
    int col;
    if (_emitterSide == LaserSide::LEFT || _emitterSide == LaserSide::RIGHT) {
        row = _emitterPos;
        col = _emitterSide == LaserSide::LEFT ? -1 : COLS;
    }
    else {
        row = _emitterSide == LaserSide::BOTTOM ? -1 : ROWS;
        col = _emitterPos;
    }

    LaserSide heading = opposite(_emitterSide);

    // A beam cannot loop (it can be retraced backwards from the edge), but the
    // limit keeps a mistake from hanging the game.
    const int limit = ROWS * COLS * 4;
    for (int i = 0; i < limit; ++i) {
        const auto [dRow, dCol] = shiftOf(heading);
        row += dRow;
        col += dCol;
        if (row < 0 || row >= ROWS || col < 0 || col >= COLS)
            return; // the beam left the grid

        const Cell& cell = _cells[row][col];
        const LaserSide from = opposite(heading);

        if (cell.state == CellState::WALL) {
            _beam.steps.push_back({ row, col, from, std::nullopt });
            return;
        }

        if (cell.state == CellState::TARGET) {
            _beam.steps.push_back({ row, col, from, std::nullopt });
            _beam.reachedTarget = true;
            return;
        }

        heading = cell.state == CellState::MIRROR ? reflect(cell, from) : heading;
        _beam.steps.push_back({ row, col, from, heading });
    }
}
