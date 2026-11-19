#include "doctest/doctest.h"

#include <set>

#include "Directions.h"

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
