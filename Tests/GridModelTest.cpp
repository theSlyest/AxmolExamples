#include "doctest/doctest.h"

#include <algorithm>
#include <vector>

#include "Directions.h"
#include "GridModel.h"

namespace {
constexpr int OWNER = 7;

int countFires(const GridModel& grid)
{
    int fires = 0;
    for (int col = 0; col < GridModel::COLS; ++col)
        for (int row = 0; row < GridModel::ROWS; ++row)
            fires += grid.isOnFire(col, row) ? 1 : 0;
    return fires;
}

// Open a straight corridor along row 1 so a blast has room to travel.
void openRow(GridModel& grid, const int lastCol)
{
    for (int col = 1; col <= lastCol; ++col)
        grid.setState(col, 1, TileState::SAFE);
}
}

TEST_SUITE("GridModel geometry") {
    TEST_CASE("the grid dimensions and blast range") {
        CHECK(GridModel::COLS == 13);
        CHECK(GridModel::ROWS == 11);
        CHECK(GridModel::BLAST_RANGE == 2);
    }

    TEST_CASE("coordsToPosition places a tile's corner on a multiple of the tile size") {
        CHECK(GridModel::coordsToPosition({ 0, 0 }) == ax::Vec2::ZERO);

        const ax::Vec2 position = GridModel::coordsToPosition({ 3, 2 });
        CHECK(position.x == doctest::Approx(3.0f * GridModel::TILE_SIZE));
        CHECK(position.y == doctest::Approx(2.0f * GridModel::TILE_SIZE));
    }

    TEST_CASE("positionToCoords returns the tile containing a position") {
        constexpr float size = GridModel::TILE_SIZE;

        CHECK(GridModel::positionToCoords({ 0.0f, 0.0f }) == GridCoords(0, 0));
        CHECK(GridModel::positionToCoords({ 3.0f * size, 2.0f * size }) == GridCoords(3, 2));
        CHECK(GridModel::positionToCoords({ 3.0f * size + 1.0f, 2.0f * size + 1.0f }) == GridCoords(3, 2));
        CHECK(GridModel::positionToCoords({ 4.0f * size - 1.0f, 3.0f * size - 1.0f }) == GridCoords(3, 2));
    }

    TEST_CASE("positionToCoords undoes coordsToPosition for every tile") {
        for (int col = 0; col < GridModel::COLS; ++col) {
            for (int row = 0; row < GridModel::ROWS; ++row) {
                const GridCoords coords(col, row);
                REQUIRE(GridModel::positionToCoords(GridModel::coordsToPosition(coords)) == coords);
            }
        }
    }

    TEST_CASE("neighborPosition moves exactly one tile in the given direction") {
        const ax::Vec2 start = GridModel::coordsToPosition({ 5, 5 });

        CHECK(GridModel::positionToCoords(GridModel::neighborPosition(start, DIR_RIGHT)) == GridCoords(6, 5));
        CHECK(GridModel::positionToCoords(GridModel::neighborPosition(start, DIR_LEFT)) == GridCoords(4, 5));
        CHECK(GridModel::positionToCoords(GridModel::neighborPosition(start, DIR_UP)) == GridCoords(5, 6));
        CHECK(GridModel::positionToCoords(GridModel::neighborPosition(start, DIR_DOWN)) == GridCoords(5, 4));
    }

    TEST_CASE("centerCoords is the tile under the middle of a tile-sized object") {
        const ax::Vec2 corner = GridModel::coordsToPosition({ 4, 3 });

        CHECK(GridModel::centerCoords(corner) == GridCoords(4, 3));
        // An object halfway into the next tile is judged by its center.
        CHECK(GridModel::centerCoords(corner + ax::Vec2(GridModel::TILE_SIZE * 0.4f, 0.0f)) == GridCoords(4, 3));
        CHECK(GridModel::centerCoords(corner + ax::Vec2(GridModel::TILE_SIZE * 0.6f, 0.0f)) == GridCoords(5, 3));
    }
}

TEST_SUITE("GridModel layout") {
    TEST_CASE("the edge and every even/even tile are steel") {
        const GridModel grid;

        for (int col = 0; col < GridModel::COLS; ++col) {
            CHECK(grid.getState(col, 0) == TileState::STEEL);
            CHECK(grid.getState(col, GridModel::ROWS - 1) == TileState::STEEL);
        }
        for (int row = 0; row < GridModel::ROWS; ++row) {
            CHECK(grid.getState(0, row) == TileState::STEEL);
            CHECK(grid.getState(GridModel::COLS - 1, row) == TileState::STEEL);
        }
        CHECK(grid.getState(2, 2) == TileState::STEEL);
        CHECK(grid.getState(4, 6) == TileState::STEEL);
    }

    TEST_CASE("each corner has a free L-shaped start and walls elsewhere") {
        const GridModel grid;

        for (const auto& [col, row] : { GridCoords(1, 1), GridCoords(1, 2), GridCoords(2, 1),
                                        GridCoords(11, 9), GridCoords(11, 8), GridCoords(10, 9) })
            CHECK(grid.getState(col, row) == TileState::SAFE);

        CHECK(grid.getState(3, 1) == TileState::WALL);
        CHECK(grid.getState(1, 3) == TileState::WALL);
        CHECK(grid.getState(5, 5) == TileState::WALL);
    }

    TEST_CASE("exactly twelve interior tiles start free") {
        const GridModel grid;

        int free = 0;
        for (int col = 0; col < GridModel::COLS; ++col)
            for (int row = 0; row < GridModel::ROWS; ++row)
                free += grid.getState(col, row) == TileState::SAFE ? 1 : 0;

        CHECK(free == 12);
    }

    TEST_CASE("states can be looked up by position") {
        const GridModel grid;

        CHECK(grid.getState(GridModel::coordsToPosition({ 1, 1 })) == TileState::SAFE);
        CHECK(grid.isSafe(GridModel::coordsToPosition({ 1, 1 }) + ax::Vec2(5.0f, 5.0f)));
        CHECK_FALSE(grid.isSafe(GridModel::coordsToPosition({ 3, 1 })));
        CHECK_FALSE(grid.isSafe(GridModel::coordsToPosition({ 2, 2 })));
    }
}

TEST_SUITE("GridModel explosions") {
    TEST_CASE("an explosion burns its tile and the free tiles next to it") {
        GridModel grid;

        grid.detonate(1, 1);

        CHECK(grid.isOnFire(1, 1));
        CHECK(grid.isOnFire(2, 1));
        CHECK(grid.isOnFire(1, 2));
    }

    TEST_CASE("fire is stopped by steel") {
        GridModel grid;

        grid.detonate(1, 1);

        CHECK(grid.getState(0, 1) == TileState::STEEL);
        CHECK_FALSE(grid.isOnFire(0, 1));
        CHECK_FALSE(grid.isOnFire(1, 0));
    }

    TEST_CASE("fire destroys the first wall it meets and goes no further") {
        GridModel grid;
        REQUIRE(grid.getState(3, 1) == TileState::WALL);
        REQUIRE(grid.getState(4, 1) == TileState::WALL);

        grid.detonate(1, 1);

        CHECK(grid.getState(3, 1) == TileState::SAFE); // destroyed
        CHECK(grid.isOnFire(3, 1));
        CHECK(grid.getState(4, 1) == TileState::WALL); // shielded by the first wall
        CHECK_FALSE(grid.isOnFire(4, 1));
    }

    TEST_CASE("fire is stopped by the first wall in every direction") {
        GridModel grid;
        grid.setState(5, 5, TileState::SAFE); // the middle of a field of walls

        grid.detonate(5, 5);

        for (const auto& [col, row] : { GridCoords(4, 5), GridCoords(6, 5), GridCoords(5, 4), GridCoords(5, 6) }) {
            CHECK(grid.getState(col, row) == TileState::SAFE); // destroyed
            CHECK(grid.isOnFire(col, row));
        }
        for (const auto& [col, row] : { GridCoords(3, 5), GridCoords(7, 5), GridCoords(5, 3), GridCoords(5, 7) }) {
            CHECK(grid.getState(col, row) == TileState::WALL); // shielded
            CHECK_FALSE(grid.isOnFire(col, row));
        }
    }

    TEST_CASE("the blast reaches BLAST_RANGE tiles and no more") {
        GridModel grid;
        openRow(grid, 8);

        grid.detonate(1, 1);

        CHECK(grid.isOnFire(2, 1));
        CHECK(grid.isOnFire(3, 1));
        CHECK_FALSE(grid.isOnFire(4, 1));
    }

    TEST_CASE("a bomb explodes after its delay") {
        GridModel grid;
        grid.setBomb(1, 1, OWNER);
        REQUIRE(grid.getState(1, 1) == TileState::BOMB);

        grid.update(GridModel::BOMB_DELAY - 0.1f);
        CHECK(grid.getState(1, 1) == TileState::BOMB);
        CHECK_FALSE(grid.isOnFire(1, 1));

        grid.update(0.2f);
        CHECK(grid.getState(1, 1) == TileState::SAFE);
        CHECK(grid.isOnFire(1, 1));
        CHECK(grid.isOnFire(2, 1));
    }

    TEST_CASE("fire burns out after its delay") {
        GridModel grid;
        grid.detonate(1, 1);
        REQUIRE(countFires(grid) > 0);

        grid.update(GridModel::FIRE_DELAY - 0.1f);
        CHECK(countFires(grid) > 0);

        grid.update(0.2f);
        CHECK(countFires(grid) == 0);
    }

    TEST_CASE("an explosion sets off the bombs it reaches") {
        GridModel grid;
        openRow(grid, 2);
        grid.setBomb(3, 1, OWNER); // two tiles from (1, 1), inside the blast range
        REQUIRE(grid.getState(4, 1) == TileState::WALL);

        grid.detonate(1, 1);

        CHECK(grid.isOnFire(3, 1));
        CHECK(grid.getState(3, 1) == TileState::SAFE);
        CHECK(grid.getState(4, 1) == TileState::SAFE); // destroyed by the second explosion
        CHECK(grid.isOnFire(4, 1));
    }

    TEST_CASE("a bomb out of range is left alone") {
        GridModel grid;
        openRow(grid, 8);
        grid.setBomb(4, 1, OWNER);

        grid.detonate(1, 1);

        CHECK(grid.getState(4, 1) == TileState::BOMB);
        CHECK_FALSE(grid.isOnFire(4, 1));
    }
}

TEST_SUITE("GridModel bombs") {
    TEST_CASE("a planted bomb remembers its owner") {
        GridModel grid;
        CHECK(grid.getBombOwner(1, 1) == GridModel::NO_OWNER);

        grid.setBomb(1, 1, OWNER);

        CHECK(grid.getBombOwner(1, 1) == OWNER);
    }

    TEST_CASE("the owner is told when the bomb is gone") {
        GridModel grid;
        std::vector<int> released;
        grid.onBombReleased = [&](const int owner) { released.push_back(owner); };
        grid.setBomb(1, 1, OWNER);

        grid.update(GridModel::BOMB_DELAY + 0.1f);

        REQUIRE(released.size() == 1);
        CHECK(released[0] == OWNER);
        CHECK(grid.getBombOwner(1, 1) == GridModel::NO_OWNER);
    }

    TEST_CASE("a bomb set off by another explosion is released too") {
        GridModel grid;
        openRow(grid, 2);
        int released = 0;
        grid.onBombReleased = [&](int) { ++released; };
        grid.setBomb(3, 1, OWNER);

        grid.detonate(1, 1);

        CHECK(released == 1);
    }

    TEST_CASE("reset releases the bombs and puts out the fires") {
        GridModel grid;
        int released = 0;
        grid.onBombReleased = [&](int) { ++released; };
        grid.setBomb(1, 1, OWNER);
        grid.detonate(1, 2);
        grid.setState(5, 5, TileState::SAFE);

        grid.reset();

        CHECK(released >= 1);
        CHECK(countFires(grid) == 0);
        CHECK(grid.getState(1, 1) == TileState::SAFE);
        CHECK(grid.getState(3, 1) == TileState::WALL);
        CHECK(grid.getState(5, 5) == TileState::WALL);
    }

    TEST_CASE("reset leaves steel alone") {
        GridModel grid;

        grid.reset();

        CHECK(grid.getState(0, 0) == TileState::STEEL);
        CHECK(grid.getState(2, 2) == TileState::STEEL);
    }
}

TEST_SUITE("GridModel notifications") {
    TEST_CASE("changes to a tile are reported") {
        GridModel grid;
        std::vector<GridCoords> changed;
        grid.onTileChanged = [&](const int col, const int row) { changed.emplace_back(col, row); };

        grid.setBomb(1, 1, OWNER);

        REQUIRE_FALSE(changed.empty());
        CHECK(changed.back() == GridCoords(1, 1));
    }

    TEST_CASE("an explosion reports every tile it touches") {
        GridModel grid;
        std::vector<GridCoords> changed;
        grid.onTileChanged = [&](const int col, const int row) { changed.emplace_back(col, row); };

        grid.detonate(1, 1);

        const auto reported = [&](const GridCoords& coords) {
            return std::find(changed.begin(), changed.end(), coords) != changed.end();
        };
        CHECK(reported({ 1, 1 }));
        CHECK(reported({ 2, 1 }));
        CHECK(reported({ 1, 2 }));
    }
}
