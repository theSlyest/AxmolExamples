#include "doctest/doctest.h"

#include <set>

#include "BomberGrid.h"
#include "Directions.h"

// These tests only use the grid's static helpers and the direction constants,
// so they need no nodes and no window.

TEST_SUITE("Directions") {
    TEST_CASE("the four directions are distinct unit vectors") {
        std::set<std::pair<float, float>> seen;
        for (const auto& direction : DIRECTIONS) {
            CHECK(direction.length() == doctest::Approx(1.0f));
            seen.insert({ direction.x, direction.y });
        }

        CHECK(seen.size() == 4);
    }

    TEST_CASE("opposite directions cancel out") {
        CHECK(DIR_UP + DIR_DOWN == ax::Vec2::ZERO);
        CHECK(DIR_LEFT + DIR_RIGHT == ax::Vec2::ZERO);
    }

    TEST_CASE("up and right point toward increasing coordinates") {
        CHECK(DIR_UP.y > 0.0f);
        CHECK(DIR_RIGHT.x > 0.0f);
        CHECK(DIR_DOWN.y < 0.0f);
        CHECK(DIR_LEFT.x < 0.0f);
    }
}

TEST_SUITE("BomberGrid geometry") {
    TEST_CASE("the grid dimensions and blast range") {
        CHECK(BomberGrid::COLS == 13);
        CHECK(BomberGrid::ROWS == 11);
        CHECK(BomberGrid::BLAST_RANGE == 2);
    }

    TEST_CASE("coordsToPosition places a tile's corner on a multiple of the tile size") {
        const ax::Vec2 origin = BomberGrid::coordsToPosition({ 0, 0 });
        CHECK(origin == ax::Vec2::ZERO);

        const ax::Vec2 position = BomberGrid::coordsToPosition({ 3, 2 });
        CHECK(position.x == doctest::Approx(3.0f * GridTile::SIZE));
        CHECK(position.y == doctest::Approx(2.0f * GridTile::SIZE));
    }

    TEST_CASE("positionToCoords returns the tile containing a position") {
        constexpr float size = GridTile::SIZE;

        CHECK(BomberGrid::positionToCoords({ 0.0f, 0.0f }) == GridCoords(0, 0));
        CHECK(BomberGrid::positionToCoords({ 3.0f * size, 2.0f * size }) == GridCoords(3, 2));
        // Anywhere inside the tile gives the same coordinates.
        CHECK(BomberGrid::positionToCoords({ 3.0f * size + 1.0f, 2.0f * size + 1.0f }) == GridCoords(3, 2));
        CHECK(BomberGrid::positionToCoords({ 4.0f * size - 1.0f, 3.0f * size - 1.0f }) == GridCoords(3, 2));
    }

    TEST_CASE("positionToCoords undoes coordsToPosition for every tile") {
        for (int col = 0; col < BomberGrid::COLS; ++col) {
            for (int row = 0; row < BomberGrid::ROWS; ++row) {
                const GridCoords coords(col, row);
                REQUIRE(BomberGrid::positionToCoords(BomberGrid::coordsToPosition(coords)) == coords);
            }
        }
    }

    TEST_CASE("neighborPosition moves exactly one tile in the given direction") {
        const ax::Vec2 start = BomberGrid::coordsToPosition({ 5, 5 });

        CHECK(BomberGrid::positionToCoords(BomberGrid::neighborPosition(start, DIR_RIGHT)) == GridCoords(6, 5));
        CHECK(BomberGrid::positionToCoords(BomberGrid::neighborPosition(start, DIR_LEFT)) == GridCoords(4, 5));
        CHECK(BomberGrid::positionToCoords(BomberGrid::neighborPosition(start, DIR_UP)) == GridCoords(5, 6));
        CHECK(BomberGrid::positionToCoords(BomberGrid::neighborPosition(start, DIR_DOWN)) == GridCoords(5, 4));
    }

    TEST_CASE("moving to a neighbor and back returns to the start") {
        const ax::Vec2 start = BomberGrid::coordsToPosition({ 7, 4 });

        for (const auto& direction : DIRECTIONS) {
            const ax::Vec2 there = BomberGrid::neighborPosition(start, direction);
            const ax::Vec2 back = BomberGrid::neighborPosition(there, direction * -1.0f);
            CHECK(back.x == doctest::Approx(start.x));
            CHECK(back.y == doctest::Approx(start.y));
        }
    }
}
