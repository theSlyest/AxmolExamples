#include "doctest/doctest.h"

#include <set>

#include "Directions.h"
#include "GridModel.h"
#include "RandomWalkBrain.h"

namespace {
// The player's corner: only up and right are free.
const ax::Vec2 CORNER = GridModel::coordsToPosition({ 1, 1 });

size_t first(const size_t) { return 0; }
size_t last(const size_t count) { return count - 1; }
}

TEST_SUITE("RandomWalkBrain") {
    TEST_CASE("it only offers the free directions to the random pick") {
        GridModel grid;
        size_t offered = 0;
        RandomWalkBrain brain([&](const size_t count) { offered = count; return size_t{ 0 }; });

        brain.chooseDirection(grid, CORNER);

        CHECK(offered == 2); // up and right; left and down are steel
    }

    TEST_CASE("the pick selects among the free directions") {
        GridModel grid;

        const ax::Vec2 a = RandomWalkBrain(first).chooseDirection(grid, CORNER);
        const ax::Vec2 b = RandomWalkBrain(last).chooseDirection(grid, CORNER);

        CHECK(a != b);
        for (const auto& direction : { a, b })
            CHECK((direction == DIR_UP || direction == DIR_RIGHT));
    }

    TEST_CASE("it stays still when it is boxed in, without calling the random pick") {
        GridModel grid;
        grid.setState(2, 1, TileState::WALL);
        grid.setState(1, 2, TileState::WALL);
        bool called = false;
        RandomWalkBrain brain([&](const size_t) { called = true; return size_t{ 0 }; });

        CHECK(brain.chooseDirection(grid, CORNER) == ax::Vec2::ZERO);
        CHECK_FALSE(called);
    }

    TEST_CASE("it does not walk into a bomb") {
        GridModel grid;
        grid.setBomb(2, 1, 0);

        const ax::Vec2 direction = RandomWalkBrain(last).chooseDirection(grid, CORNER);

        CHECK(direction == DIR_UP);
    }

    TEST_CASE("with the default random source it only picks free directions, and picks them all") {
        const GridModel grid;
        RandomWalkBrain brain;
        std::set<std::pair<float, float>> seen;

        for (int i = 0; i < 200; ++i) {
            const ax::Vec2 direction = brain.chooseDirection(grid, CORNER);
            REQUIRE(grid.isSafe(GridModel::neighborPosition(CORNER, direction)));
            seen.insert({ direction.x, direction.y });
        }

        CHECK(seen.size() == 2);
    }
}
