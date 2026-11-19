#include "doctest/doctest.h"

#include "BalancerGame.h"

namespace {
constexpr float FRAME = 1.0f / 60.0f;

float middle(const float min, const float max)
{
    return (min + max) / 2.0f;
}

BalancerGame::Config config()
{
    BalancerGame::Config result;
    result.generatorRates = { 0.5f, 0.25f, 0.125f };
    return result;
}

// Run the game for `seconds` of simulated time or until it ends.
void run(BalancerGame& game, const float seconds)
{
    for (float t = 0.0f; t < seconds && !game.isOver(); t += FRAME)
        game.update(FRAME);
}

// Leave the generators alone until the game ends.
void runUntilOver(BalancerGame& game)
{
    run(game, 600.0f);
}
}

TEST_SUITE("BalancerGame") {
    TEST_CASE("a new game starts with full strikes, no uptime and the start level") {
        BalancerGame game(config(), middle);
        game.reset();

        CHECK_FALSE(game.isOver());
        CHECK(game.getStrikes() == 3);
        CHECK(game.getUpTime() == doctest::Approx(0.0f));
        CHECK(game.getSupply() == doctest::Approx(0.5f));
        CHECK(game.getDemand() == doctest::Approx(0.5f));
        CHECK(game.getGeneratorCount() == 3);
    }

    TEST_CASE("reset tells the view about the initial state") {
        BalancerGame game(config(), middle);
        int strikes = -1;
        float upTime = -1.0f;
        bool graceActive = true;
        game.onStrikesChanged = [&](const int value) { strikes = value; };
        game.onUpTimeChanged = [&](const float value) { upTime = value; };
        game.onGraceChanged = [&](const bool active, float) { graceActive = active; };

        game.reset();

        CHECK(strikes == 3);
        CHECK(upTime == doctest::Approx(0.0f));
        CHECK_FALSE(graceActive);
    }

    TEST_CASE("uptime grows while the supply is inside the tolerance band") {
        BalancerGame game(config(), middle);
        game.reset();

        run(game, 1.0f); // the demand starts at the supply and drifts slowly

        CHECK(game.getUpTime() == doctest::Approx(1.0f).epsilon(0.05));
        CHECK(game.getStrikes() == 3);
    }

    TEST_CASE("raising a generator raises the supply, which is their average") {
        BalancerGame game(config(), middle);
        game.reset();

        game.increase(0);
        run(game, 1.0f); // the fastest generator needs half a second per half range

        CHECK(game.getGeneratorValue(0) == doctest::Approx(0.75f));
        CHECK(game.getGeneratorValue(1) == doctest::Approx(0.5f));
        CHECK(game.getSupply() == doctest::Approx((0.75f + 0.5f + 0.5f) / 3.0f));
    }

    TEST_CASE("time outside the band costs strikes and ends the game once") {
        BalancerGame game(config(), middle);
        int gameOvers = 0;
        int lastStrikes = 3;
        bool sawGrace = false;
        game.onGameOver = [&] { ++gameOvers; };
        game.onStrikesChanged = [&](const int value) { lastStrikes = value; };
        game.onGraceChanged = [&](const bool active, float) { sawGrace = sawGrace || active; };
        game.reset();

        runUntilOver(game);

        CHECK(game.isOver());
        CHECK(game.getStrikes() == 0);
        CHECK(lastStrikes == 0);
        CHECK(gameOvers == 1);
        CHECK(sawGrace);
    }

    TEST_CASE("the game is frozen once it is over") {
        BalancerGame game(config(), middle);
        game.reset();
        runUntilOver(game);

        const float upTime = game.getUpTime();
        const float value = game.getGeneratorValue(0);
        game.increase(0);
        game.update(5.0f);

        CHECK(game.getUpTime() == doctest::Approx(upTime));
        CHECK(game.getGeneratorValue(0) == doctest::Approx(value));
    }

    TEST_CASE("reset starts a fresh game after game over") {
        BalancerGame game(config(), middle);
        game.reset();
        runUntilOver(game);
        REQUIRE(game.isOver());
        REQUIRE(game.getUpTime() > 0.0f);

        game.reset();

        CHECK_FALSE(game.isOver());
        CHECK(game.getStrikes() == 3);
        CHECK(game.getUpTime() == doctest::Approx(0.0f));
    }

    TEST_CASE("the config controls the number of strikes") {
        auto settings = config();
        settings.strikes = 1;
        BalancerGame game(settings, middle);
        game.reset();

        CHECK(game.getStrikes() == 1);
        runUntilOver(game);
        CHECK(game.isOver());
    }
}
