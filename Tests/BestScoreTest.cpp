#include "doctest/doctest.h"

#include <vector>

#include "BestScore.h"

namespace {
// A best score whose storage is a list of saved values, so nothing real is touched.
// `stored` is what a previous run left behind (or -1 for nothing).
struct Fixture
{
    std::vector<int> saves;
    BestScore score;

    explicit Fixture(const int startValue, const int stored = -1)
        : score(
              startValue,
              [stored](const int start) { return stored >= 0 ? stored : start; },
              [this](const int best) { saves.push_back(best); })
    {
    }
};
}

TEST_SUITE("BestScore") {
    TEST_CASE("without a saved result, the best is the start value") {
        Fixture f(5);

        CHECK(f.score.get() == 5);
        CHECK(f.saves.empty());
    }

    TEST_CASE("a saved result from a previous run is used instead of the start value") {
        Fixture f(5, 3);

        CHECK(f.score.get() == 3);
    }

    TEST_CASE("fewer mirrors than the best is a record, and it is saved") {
        Fixture f(5);

        CHECK(f.score.submit(4));

        CHECK(f.score.get() == 4);
        REQUIRE(f.saves.size() == 1);
        CHECK(f.saves[0] == 4);
    }

    TEST_CASE("the same number of mirrors is not a record") {
        Fixture f(5, 3);

        CHECK_FALSE(f.score.submit(3));

        CHECK(f.score.get() == 3);
        CHECK(f.saves.empty());
    }

    TEST_CASE("solving with the start value's number of mirrors is not a record") {
        Fixture f(5);

        CHECK_FALSE(f.score.submit(5));

        CHECK(f.score.get() == 5);
        CHECK(f.saves.empty());
    }

    TEST_CASE("more mirrors than the best is not a record and changes nothing") {
        Fixture f(5, 2);

        CHECK_FALSE(f.score.submit(4));

        CHECK(f.score.get() == 2);
        CHECK(f.saves.empty());
    }

    TEST_CASE("only the records are saved, in order") {
        Fixture f(5);

        CHECK(f.score.submit(4));
        CHECK_FALSE(f.score.submit(6));
        CHECK(f.score.submit(3));
        CHECK_FALSE(f.score.submit(3));
        CHECK(f.score.submit(2));

        CHECK(f.score.get() == 2);
        CHECK(f.saves == std::vector<int>{ 4, 3, 2 });
    }
}
