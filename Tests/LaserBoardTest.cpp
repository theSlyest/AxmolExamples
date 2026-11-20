#include "doctest/doctest.h"

#include <string>
#include <vector>

#include "LaserBoard.h"

namespace {
const std::string EMPTY_ROW = "............";

// A grid of empty cells.
std::vector<std::string> emptyLayout()
{
    return std::vector<std::string>(LaserBoard::ROWS, EMPTY_ROW);
}

// Replace one cell of a layout; `row` counts from the bottom.
std::vector<std::string> with(std::vector<std::string> layout, const int row, const int col, const char symbol)
{
    layout[LaserBoard::ROWS - 1 - row][col] = symbol;
    return layout;
}

// The puzzle of the game: the target sits in a pocket of walls and the beam
// must come at it from the left.
std::vector<std::string> gameLayout()
{
    return {
        "............",
        "............",
        "............",
        ".....#......",
        "..#..T#.....",
        "...#.#......",
        "....#.......",
        "............",
    };
}

// Click a cell as many times as needed to make it a mirror of the wanted kind.
void placeMirror(LaserBoard& board, const int row, const int col, const bool backslash)
{
    board.cycleMirror(row, col);
    if (backslash)
        board.cycleMirror(row, col);
}

LaserBoard leftBoard(const std::vector<std::string>& layout, const int row = 2, const int maxMirrors = 5)
{
    return LaserBoard(layout, LaserSide::LEFT, row, maxMirrors);
}
}

TEST_SUITE("LaserBoard layout") {
    TEST_CASE("the layout is read from the top row down") {
        LaserBoard board = leftBoard(gameLayout(), 6);

        CHECK(board.getState(3, 5) == CellState::TARGET);
        CHECK(board.getState(3, 2) == CellState::WALL);
        CHECK(board.getState(3, 6) == CellState::WALL);
        CHECK(board.getState(4, 5) == CellState::WALL);
        CHECK(board.getState(1, 4) == CellState::WALL);
        CHECK(board.getState(0, 0) == CellState::EMPTY);
        CHECK(board.getState(7, 11) == CellState::EMPTY);
    }

    TEST_CASE("a new board has no mirrors") {
        LaserBoard board = leftBoard(emptyLayout());

        CHECK(board.getMirrorCount() == 0);
        CHECK(board.getRemainingMirrors() == 5);
        CHECK(board.getMaxMirrors() == 5);
    }

    TEST_CASE("the emitter's cell is the first cell the beam enters") {
        CHECK(LaserBoard(emptyLayout(), LaserSide::LEFT, 6, 5).isEmitterCell(6, 0));
        CHECK(LaserBoard(emptyLayout(), LaserSide::RIGHT, 3, 5).isEmitterCell(3, LaserBoard::COLS - 1));
        CHECK(LaserBoard(emptyLayout(), LaserSide::BOTTOM, 4, 5).isEmitterCell(0, 4));
        CHECK(LaserBoard(emptyLayout(), LaserSide::TOP, 4, 5).isEmitterCell(LaserBoard::ROWS - 1, 4));

        LaserBoard board = leftBoard(emptyLayout(), 6);
        CHECK_FALSE(board.isEmitterCell(5, 0));
        CHECK_FALSE(board.isEmitterCell(6, 1));
    }

    TEST_CASE("opposite sides") {
        CHECK(LaserBoard::opposite(LaserSide::LEFT) == LaserSide::RIGHT);
        CHECK(LaserBoard::opposite(LaserSide::RIGHT) == LaserSide::LEFT);
        CHECK(LaserBoard::opposite(LaserSide::TOP) == LaserSide::BOTTOM);
        CHECK(LaserBoard::opposite(LaserSide::BOTTOM) == LaserSide::TOP);
    }
}

TEST_SUITE("LaserBoard beam") {
    TEST_CASE("with nothing in the way the beam crosses the whole row and leaves") {
        LaserBoard board = leftBoard(emptyLayout(), 2);
        const auto& beam = board.getBeam();

        REQUIRE(beam.steps.size() == LaserBoard::COLS);
        for (int col = 0; col < LaserBoard::COLS; ++col) {
            CHECK(beam.steps[col].row == 2);
            CHECK(beam.steps[col].col == col);
            CHECK(beam.steps[col].from == LaserSide::LEFT);
            CHECK(beam.steps[col].to == LaserSide::RIGHT);
        }
        CHECK_FALSE(beam.reachedTarget);
        CHECK_FALSE(board.isSolved());
    }

    TEST_CASE("a wall stops the beam, and the wall's step has no exit") {
        LaserBoard board = leftBoard(with(emptyLayout(), 2, 5, '#'), 2);
        const auto& beam = board.getBeam();

        REQUIRE(beam.steps.size() == 6); // five empty cells and the wall
        CHECK(beam.steps.back().col == 5);
        CHECK_FALSE(beam.steps.back().to.has_value());
        CHECK_FALSE(beam.reachedTarget);
    }

    TEST_CASE("the target stops the beam and solves the puzzle") {
        LaserBoard board = leftBoard(with(emptyLayout(), 2, 7, 'T'), 2);
        const auto& beam = board.getBeam();

        REQUIRE(beam.steps.size() == 8);
        CHECK(beam.steps.back().col == 7);
        CHECK_FALSE(beam.steps.back().to.has_value());
        CHECK(beam.reachedTarget);
        CHECK(board.isSolved());
    }

    TEST_CASE("the beam enters from the emitter's side whichever side that is") {
        LaserBoard right(emptyLayout(), LaserSide::RIGHT, 2, 5);
        REQUIRE(right.getBeam().steps.size() == LaserBoard::COLS);
        CHECK(right.getBeam().steps.front().col == LaserBoard::COLS - 1);
        CHECK(right.getBeam().steps.front().from == LaserSide::RIGHT);
        CHECK(right.getBeam().steps.back().col == 0);

        LaserBoard top(emptyLayout(), LaserSide::TOP, 4, 5);
        REQUIRE(top.getBeam().steps.size() == LaserBoard::ROWS);
        CHECK(top.getBeam().steps.front().row == LaserBoard::ROWS - 1);
        CHECK(top.getBeam().steps.front().from == LaserSide::TOP);
        CHECK(top.getBeam().steps.back().row == 0);

        LaserBoard bottom(emptyLayout(), LaserSide::BOTTOM, 4, 5);
        REQUIRE(bottom.getBeam().steps.size() == LaserBoard::ROWS);
        CHECK(bottom.getBeam().steps.front().row == 0);
        CHECK(bottom.getBeam().steps.front().from == LaserSide::BOTTOM);
        CHECK(bottom.getBeam().steps.back().row == LaserBoard::ROWS - 1);
    }

    TEST_CASE("a slash mirror sends a beam coming from the left upward") {
        LaserBoard board = leftBoard(emptyLayout(), 2);
        placeMirror(board, 2, 3, false); // "/"

        const auto& steps = board.getBeam().steps;
        // Cells (2,0) (2,1) (2,2), the mirror, then up the column to the top edge.
        REQUIRE(steps.size() == 4 + (LaserBoard::ROWS - 3));
        CHECK(steps[3].row == 2);
        CHECK(steps[3].col == 3);
        CHECK(steps[3].from == LaserSide::LEFT);
        CHECK(steps[3].to == LaserSide::TOP);
        CHECK(steps[4].row == 3);
        CHECK(steps[4].col == 3);
        CHECK(steps[4].from == LaserSide::BOTTOM);
        CHECK(steps.back().row == LaserBoard::ROWS - 1);
    }

    TEST_CASE("a backslash mirror sends a beam coming from the left downward") {
        LaserBoard board = leftBoard(emptyLayout(), 2);
        placeMirror(board, 2, 3, true); // "\"

        const auto& steps = board.getBeam().steps;
        REQUIRE(steps.size() == 4 + 2); // down to row 0
        CHECK(steps[3].from == LaserSide::LEFT);
        CHECK(steps[3].to == LaserSide::BOTTOM);
        CHECK(steps.back().row == 0);
        CHECK(steps.back().col == 3);
    }

    TEST_CASE("a mirror reflects a beam from every side") {
        struct Case { bool backslash; LaserSide emitter; LaserSide expectedExit; };
        // The mirror is in the middle of its row or column; the beam enters from the emitter's side.
        const Case cases[] = {
            { false, LaserSide::LEFT,   LaserSide::TOP },
            { false, LaserSide::TOP,    LaserSide::LEFT },
            { false, LaserSide::RIGHT,  LaserSide::BOTTOM },
            { false, LaserSide::BOTTOM, LaserSide::RIGHT },
            { true,  LaserSide::LEFT,   LaserSide::BOTTOM },
            { true,  LaserSide::BOTTOM, LaserSide::LEFT },
            { true,  LaserSide::RIGHT,  LaserSide::TOP },
            { true,  LaserSide::TOP,    LaserSide::RIGHT },
        };

        for (const Case& c : cases) {
            // Row 3 and column 5 are away from every edge.
            const bool sideways = c.emitter == LaserSide::LEFT || c.emitter == LaserSide::RIGHT;
            LaserBoard board(emptyLayout(), c.emitter, sideways ? 3 : 5, 5);
            placeMirror(board, 3, 5, c.backslash);

            // Find the mirror's step in the beam.
            const LaserBoard::BeamStep* mirror = nullptr;
            for (const auto& step : board.getBeam().steps) {
                if (step.row == 3 && step.col == 5)
                    mirror = &step;
            }

            REQUIRE(mirror != nullptr);
            CHECK(mirror->from == c.emitter);
            CHECK(mirror->to == c.expectedExit);
        }
    }

    TEST_CASE("the beam can cross a cell it already went through") {
        // A square path: right to (2,4), down to (0,4)... then back across row 2 is not possible,
        // so make the beam cross itself: right, down, left, up through its own first leg.
        LaserBoard board = leftBoard(emptyLayout(), 4);
        placeMirror(board, 4, 6, true);  // "\" : right -> down
        placeMirror(board, 2, 6, false); // "/" : down -> left
        placeMirror(board, 2, 3, true);  // "\" : left -> up
        // Going up column 3 from row 2 crosses row 4, where the beam came from the left.

        int crossings = 0;
        for (const auto& step : board.getBeam().steps) {
            if (step.row == 4 && step.col == 3)
                ++crossings;
        }

        CHECK(crossings == 2);
        CHECK_FALSE(board.isSolved());
    }
}

TEST_SUITE("LaserBoard mirrors") {
    TEST_CASE("clicking cycles a cell through a slash, a backslash and nothing") {
        LaserBoard board = leftBoard(emptyLayout());

        CHECK(board.cycleMirror(4, 4));
        CHECK(board.getState(4, 4) == CellState::MIRROR);
        CHECK_FALSE(board.isBackslash(4, 4));
        CHECK(board.getMirrorCount() == 1);

        CHECK(board.cycleMirror(4, 4));
        CHECK(board.getState(4, 4) == CellState::MIRROR);
        CHECK(board.isBackslash(4, 4));
        CHECK(board.getMirrorCount() == 1);

        CHECK(board.cycleMirror(4, 4));
        CHECK(board.getState(4, 4) == CellState::EMPTY);
        CHECK(board.getMirrorCount() == 0);

        // And around again, starting with a slash.
        CHECK(board.cycleMirror(4, 4));
        CHECK_FALSE(board.isBackslash(4, 4));
    }

    TEST_CASE("the number of mirrors is limited, but placed mirrors can still be turned and removed") {
        LaserBoard board = leftBoard(emptyLayout(), 0, 2);

        CHECK(board.cycleMirror(4, 1));
        CHECK(board.cycleMirror(4, 2));
        CHECK(board.getRemainingMirrors() == 0);

        CHECK_FALSE(board.cycleMirror(4, 3)); // no mirror left
        CHECK(board.getState(4, 3) == CellState::EMPTY);
        CHECK(board.getMirrorCount() == 2);

        CHECK(board.cycleMirror(4, 1)); // turn one
        CHECK(board.cycleMirror(4, 1)); // remove it
        CHECK(board.getRemainingMirrors() == 1);
        CHECK(board.cycleMirror(4, 3)); // now there is room
    }

    TEST_CASE("walls, the target and the emitter's cell cannot hold a mirror") {
        auto layout = with(with(emptyLayout(), 4, 4, '#'), 4, 6, 'T');
        LaserBoard board = leftBoard(layout, 2);

        CHECK_FALSE(board.cycleMirror(4, 4));
        CHECK(board.getState(4, 4) == CellState::WALL);
        CHECK_FALSE(board.cycleMirror(4, 6));
        CHECK(board.getState(4, 6) == CellState::TARGET);
        CHECK_FALSE(board.cycleMirror(2, 0)); // the emitter's cell
        CHECK(board.getState(2, 0) == CellState::EMPTY);
        CHECK(board.getMirrorCount() == 0);
    }

    TEST_CASE("cells outside the grid are ignored") {
        LaserBoard board = leftBoard(emptyLayout());

        CHECK_FALSE(board.cycleMirror(-1, 3));
        CHECK_FALSE(board.cycleMirror(3, -1));
        CHECK_FALSE(board.cycleMirror(LaserBoard::ROWS, 3));
        CHECK_FALSE(board.cycleMirror(3, LaserBoard::COLS));
    }

    TEST_CASE("reset takes the mirrors off") {
        LaserBoard board = leftBoard(emptyLayout());
        placeMirror(board, 2, 3, true);
        placeMirror(board, 4, 4, false);
        REQUIRE(board.getMirrorCount() == 2);

        board.reset();

        CHECK(board.getMirrorCount() == 0);
        CHECK(board.getRemainingMirrors() == 5);
        CHECK(board.getState(2, 3) == CellState::EMPTY);
        CHECK(board.getBeam().steps.size() == LaserBoard::COLS); // straight again
    }

    TEST_CASE("reset keeps the walls and the target") {
        LaserBoard board = leftBoard(gameLayout(), 6);
        placeMirror(board, 6, 3, true);

        board.reset();

        CHECK(board.getState(3, 5) == CellState::TARGET);
        CHECK(board.getState(4, 5) == CellState::WALL);
    }
}

TEST_SUITE("LaserBoard puzzle") {
    TEST_CASE("the game's puzzle starts unsolved: the beam crosses the grid") {
        LaserBoard board = leftBoard(gameLayout(), 6);

        CHECK_FALSE(board.isSolved());
        CHECK(board.getBeam().steps.size() == LaserBoard::COLS);
    }

    TEST_CASE("two backslash mirrors solve the game's puzzle") {
        LaserBoard board = leftBoard(gameLayout(), 6);

        placeMirror(board, 6, 3, true); // sends the beam down column 3
        CHECK_FALSE(board.isSolved());
        placeMirror(board, 3, 3, true); // sends it right, into the target

        CHECK(board.isSolved());
        CHECK(board.getMirrorCount() == 2);
        CHECK(board.getBeam().steps.back().row == 3);
        CHECK(board.getBeam().steps.back().col == 5);
    }

    TEST_CASE("changing a mirror on the beam's path can unsolve the puzzle") {
        LaserBoard board = leftBoard(gameLayout(), 6);
        placeMirror(board, 6, 3, true);
        placeMirror(board, 3, 3, true);
        REQUIRE(board.isSolved());

        board.cycleMirror(3, 3); // "\" -> nothing

        CHECK_FALSE(board.isSolved());
    }

    TEST_CASE("a mirror away from the beam changes nothing") {
        LaserBoard board = leftBoard(gameLayout(), 6);
        const size_t steps = board.getBeam().steps.size();

        board.cycleMirror(0, 0);

        CHECK(board.getBeam().steps.size() == steps);
        CHECK_FALSE(board.isSolved());
    }

    TEST_CASE("a beam that hits a wall does not solve the puzzle") {
        LaserBoard board = leftBoard(gameLayout(), 6);
        placeMirror(board, 6, 5, true); // down column 5 into the wall at (4, 5)

        CHECK_FALSE(board.isSolved());
        CHECK(board.getBeam().steps.back().row == 4);
        CHECK(board.getBeam().steps.back().col == 5);
        CHECK_FALSE(board.getBeam().steps.back().to.has_value());
    }
}
