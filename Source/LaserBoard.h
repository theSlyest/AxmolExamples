#pragma once

#include <optional>
#include <string>
#include <vector>

enum class CellState { EMPTY, MIRROR, WALL, TARGET };

// A side of a cell, or of the grid.
enum class LaserSide { LEFT, BOTTOM, RIGHT, TOP };

// The puzzle: a grid of walls and a target, an emitter on one side, and the
// mirrors the player has placed. It works out where the beam goes.
//
// This class has no nodes and no drawing, so it can be read and tested on
// its own. Rows are counted from the bottom (row 0) and columns from the left
// (column 0), like the positions on the screen.
class LaserBoard
{
public:
    static constexpr int ROWS = 8;
    static constexpr int COLS = 12;

    // One cell the beam crosses: it enters through side `from` and leaves
    // through side `to`. A wall or the target stops the beam, so `to` is empty.
    struct BeamStep
    {
        int row;
        int col;
        LaserSide from;
        std::optional<LaserSide> to;
    };

    struct Beam
    {
        std::vector<BeamStep> steps;
        bool reachedTarget = false;
    };

    // `layout` has ROWS strings of COLS characters, from the top row down:
    // '.' for an empty cell, '#' for a wall and 'T' for the target. The emitter
    // sits outside the grid on `emitterSide`, in front of row (or column)
    // `emitterPos`. At most `maxMirrors` mirrors can be placed.
    LaserBoard(const std::vector<std::string>& layout, LaserSide emitterSide, int emitterPos, int maxMirrors);

    static LaserSide opposite(LaserSide side);

    // Remove every mirror.
    void reset();

    [[nodiscard]] CellState getState(int row, int col) const { return _cells[row][col].state; }

    // For a mirror: true if it runs from the top left to the bottom right ("\"),
    // false if it runs from the bottom left to the top right ("/").
    [[nodiscard]] bool isBackslash(int row, int col) const { return _cells[row][col].backslash; }

    // The first cell the beam enters. It cannot hold a mirror.
    [[nodiscard]] bool isEmitterCell(int row, int col) const;

    // Click on a cell: an empty cell gets a "/" mirror, which turns into a "\"
    // mirror and then goes away. Returns true if the cell changed. Nothing happens
    // on walls, the target or the emitter's cell, or when all the mirrors are used
    // and the cell is empty.
    bool cycleMirror(int row, int col);

    [[nodiscard]] int getMirrorCount() const { return _mirrorCount; }
    [[nodiscard]] int getMaxMirrors() const { return _maxMirrors; }
    [[nodiscard]] int getRemainingMirrors() const { return _maxMirrors - _mirrorCount; }

    [[nodiscard]] LaserSide getEmitterSide() const { return _emitterSide; }
    [[nodiscard]] int getEmitterPos() const { return _emitterPos; }

    // The beam with the mirrors as they are now.
    [[nodiscard]] const Beam& getBeam() const { return _beam; }
    [[nodiscard]] bool isSolved() const { return _beam.reachedTarget; }

private:
    struct Cell
    {
        CellState state = CellState::EMPTY;
        bool backslash = false;
    };

    std::vector<std::vector<Cell>> _cells;
    LaserSide _emitterSide;
    int _emitterPos;
    int _maxMirrors;
    int _mirrorCount = 0;
    Beam _beam;

    // The side a beam leaves a mirror cell by, given the side it entered by.
    [[nodiscard]] LaserSide reflect(const Cell& mirror, LaserSide from) const;
    void traceBeam();
};
