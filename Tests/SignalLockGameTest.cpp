#include "doctest/doctest.h"

#include <algorithm>
#include <cmath>
#include <vector>

#include "SignalLockGame.h"

namespace {
constexpr float FRAME = 1.0f / 60.0f;

// A random source that returns the given numbers in order, then starts over.
SignalLockGame::Random sequence(std::vector<float> values)
{
    return [values = std::move(values), next = size_t{ 0 }]() mutable {
        const float value = values[next];
        next = (next + 1) % values.size();
        return value;
    };
}

// Run the game for `seconds` at 60 frames per second with `direction` held.
void run(SignalLockGame& game, const float seconds, const int direction)
{
    for (float t = 0.0f; t < seconds; t += FRAME)
        game.update(FRAME, direction);
}

// Tune toward the station until the dial is right on it, then stop.
void tuneOnto(SignalLockGame& game)
{
    for (int i = 0; i < 600 && std::abs(game.getDialValue() - game.getStation()) > 0.001f; ++i) {
        const int direction = game.getStation() > game.getDialValue() ? 1 : -1;
        // A small last step so the dial does not overshoot the station.
        const float distance = std::abs(game.getDialValue() - game.getStation());
        game.update(std::min(FRAME, distance / SignalLockGame::TUNE_SPEED), direction);
    }
}
}

TEST_SUITE("SignalLockGame start") {
    TEST_CASE("a new round starts with no score, full time and the dial in the middle") {
        SignalLockGame game;
        int score = -1;
        int time = -1;
        game.onScoreChanged = [&](const int value) { score = value; };
        game.onTimeChanged = [&](const int value) { time = value; };

        game.reset();

        CHECK_FALSE(game.isPlaying());
        CHECK_FALSE(game.isOver());
        CHECK(game.getScore() == 0);
        CHECK(game.getTime() == SignalLockGame::START_TIME);
        CHECK(game.getDialValue() == doctest::Approx(SignalLockGame::START_VALUE));
        CHECK(score == 0);
        CHECK(time == SignalLockGame::START_TIME);
    }

    TEST_CASE("nothing happens, and the clock waits, until a tuning key is pressed") {
        SignalLockGame game;
        game.reset();

        game.update(5.0f, 0);

        CHECK_FALSE(game.isPlaying());
        CHECK(game.getTime() == SignalLockGame::START_TIME);
    }

    TEST_CASE("the first tuning key starts the round") {
        SignalLockGame game;
        game.reset();

        game.update(FRAME, 1);

        CHECK(game.isPlaying());
    }

    TEST_CASE("a key held through a restart starts the next round right away") {
        SignalLockGame game;
        game.reset();
        game.update(FRAME, -1);
        REQUIRE(game.isPlaying());

        game.reset();
        game.update(FRAME, -1);

        CHECK(game.isPlaying());
    }
}

TEST_SUITE("SignalLockGame stations") {
    TEST_CASE("a station too close to the dial is rejected") {
        SignalLockGame game(sequence({ 0.55f, 0.6f, 0.9f }));
        game.reset();

        game.update(FRAME, 1);

        CHECK(game.getStation() == doctest::Approx(0.9f));
    }

    TEST_CASE("a station exactly LOCK_SAFE away is accepted") {
        SignalLockGame game(sequence({ 0.75f }));
        game.reset();

        game.update(FRAME, 1);

        CHECK(game.getStation() == doctest::Approx(0.75f));
    }

    TEST_CASE("a stubborn random source falls back to the far end of the dial") {
        SignalLockGame game(sequence({ 0.5f })); // always on top of the dial
        game.reset();

        game.update(FRAME, 1);

        CHECK(std::abs(game.getStation() - SignalLockGame::START_VALUE) >= SignalLockGame::LOCK_SAFE);
        CHECK((game.getStation() == doctest::Approx(0.0f) || game.getStation() == doctest::Approx(1.0f)));
    }

    TEST_CASE("with the default random source every station is far enough from the dial") {
        SignalLockGame game;

        for (int round = 0; round < 300; ++round) {
            game.reset();
            game.update(FRAME, 1);
            // Measured from where the dial was when the station was picked.
            REQUIRE(std::abs(game.getStation() - SignalLockGame::START_VALUE) >= SignalLockGame::LOCK_SAFE);
            REQUIRE(game.getStation() >= 0.0f);
            REQUIRE(game.getStation() <= 1.0f);
        }
    }
}

TEST_SUITE("SignalLockGame tuning") {
    TEST_CASE("the dial moves at TUNE_SPEED in the held direction") {
        SignalLockGame up(sequence({ 0.9f }));
        up.reset();
        run(up, 1.0f, 1);
        CHECK(up.getDialValue() == doctest::Approx(0.5f + SignalLockGame::TUNE_SPEED).epsilon(0.02));

        SignalLockGame down(sequence({ 0.9f }));
        down.reset();
        run(down, 1.0f, -1);
        CHECK(down.getDialValue() == doctest::Approx(0.5f - SignalLockGame::TUNE_SPEED).epsilon(0.02));
    }

    TEST_CASE("the dial stays put without a direction") {
        SignalLockGame game(sequence({ 0.9f }));
        game.reset();
        run(game, 0.5f, 1);
        const float value = game.getDialValue();

        run(game, 2.0f, 0);

        CHECK(game.getDialValue() == doctest::Approx(value));
    }

    TEST_CASE("the dial stops at both ends") {
        SignalLockGame game(sequence({ 0.0f, 1.0f }));
        game.reset();

        run(game, 5.0f, 1);
        CHECK(game.getDialValue() == doctest::Approx(1.0f));

        run(game, 5.0f, -1);
        CHECK(game.getDialValue() == doctest::Approx(0.0f));
    }
}

TEST_SUITE("SignalLockGame glitch") {
    TEST_CASE("the picture is fully garbled far from the station") {
        SignalLockGame game(sequence({ 0.9f })); // 0.4 away from the start
        game.reset();

        game.update(FRAME, 1);

        CHECK(game.getGlitch() == doctest::Approx(1.0f));
    }

    TEST_CASE("the picture is clear on the station") {
        SignalLockGame game(sequence({ 0.9f }));
        game.reset();
        game.update(FRAME, 1);

        tuneOnto(game);
        game.update(0.0f, 1);

        CHECK(game.getGlitch() == doctest::Approx(0.0f).epsilon(0.01));
    }

    TEST_CASE("the glitch fades in proportion to the distance inside DETECT_DIST") {
        SignalLockGame game(sequence({ 0.9f }));
        game.reset();
        game.update(FRAME, 1);

        // Halfway through the detection range.
        while (std::abs(game.getDialValue() - game.getStation()) > SignalLockGame::DETECT_DIST / 2.0f)
            game.update(FRAME, 1);

        CHECK(game.getGlitch() == doctest::Approx(0.5f).epsilon(0.1));
    }
}

TEST_SUITE("SignalLockGame locking") {
    TEST_CASE("staying on the station for LOCK_DELAY seconds locks it and scores") {
        SignalLockGame game(sequence({ 0.75f, 0.2f }));
        int score = -1;
        game.onScoreChanged = [&](const int value) { score = value; };
        game.reset();
        game.update(FRAME, 1);

        tuneOnto(game);
        run(game, SignalLockGame::LOCK_DELAY + 0.1f, 0);

        CHECK(game.getScore() == 1);
        CHECK(score == 1);
    }

    TEST_CASE("the lock builds up while on the station and resets when leaving it") {
        SignalLockGame game(sequence({ 0.75f, 0.2f }));
        game.reset();
        game.update(FRAME, 1);
        tuneOnto(game);

        run(game, 0.3f, 0);
        CHECK(game.getLockProgress() > 0.4f);
        CHECK(game.getLockProgress() < 1.0f);
        CHECK(game.getScore() == 0);

        run(game, 1.0f, -1); // tune away from the station
        CHECK(game.getLockProgress() == doctest::Approx(0.0f));
        CHECK(game.getScore() == 0);
    }

    TEST_CASE("after a lock a new station is picked away from the dial") {
        // 0.75 locks first; 0.76 is too close to the dial and is skipped; 0.2 is accepted.
        SignalLockGame game(sequence({ 0.75f, 0.76f, 0.2f }));
        game.reset();
        game.update(FRAME, 1);
        tuneOnto(game);

        run(game, SignalLockGame::LOCK_DELAY + 0.1f, 0);

        REQUIRE(game.getScore() == 1);
        CHECK(game.getStation() == doctest::Approx(0.2f));
        CHECK(game.getLockProgress() == doctest::Approx(0.0f).epsilon(0.2));
    }

    TEST_CASE("each lock scores one point") {
        SignalLockGame game(sequence({ 0.75f, 0.25f, 0.75f }));
        game.reset();
        game.update(FRAME, 1);

        for (int lock = 1; lock <= 3; ++lock) {
            tuneOnto(game);
            run(game, SignalLockGame::LOCK_DELAY + 0.1f, 0);
            CHECK(game.getScore() == lock);
        }
    }
}

TEST_SUITE("SignalLockGame clock") {
    TEST_CASE("the time counts down one second at a time once the round has started") {
        SignalLockGame game(sequence({ 0.9f }));
        int time = -1;
        game.onTimeChanged = [&](const int value) { time = value; };
        game.reset();

        game.update(0.5f, 1);
        CHECK(game.getTime() == SignalLockGame::START_TIME);

        game.update(0.6f, 1);
        CHECK(game.getTime() == SignalLockGame::START_TIME - 1);
        CHECK(time == SignalLockGame::START_TIME - 1);
    }

    TEST_CASE("the round ends once, when the time runs out") {
        SignalLockGame game(sequence({ 0.0f }));
        int gameOvers = 0;
        game.onGameOver = [&] { ++gameOvers; };
        game.reset();

        for (int second = 0; second < SignalLockGame::START_TIME + 5; ++second)
            game.update(1.0f, 1);

        CHECK(game.isOver());
        CHECK_FALSE(game.isPlaying());
        CHECK(game.getTime() == 0);
        CHECK(gameOvers == 1);
    }

    TEST_CASE("nothing changes after the round is over, and reset starts a new one") {
        SignalLockGame game(sequence({ 0.0f }));
        game.reset();
        for (int second = 0; second < SignalLockGame::START_TIME; ++second)
            game.update(1.0f, 1);
        REQUIRE(game.isOver());

        const float dial = game.getDialValue();
        const int score = game.getScore();
        game.update(5.0f, -1);
        CHECK(game.getDialValue() == doctest::Approx(dial));
        CHECK(game.getScore() == score);
        CHECK(game.getTime() == 0);

        game.reset();
        CHECK_FALSE(game.isOver());
        CHECK(game.getTime() == SignalLockGame::START_TIME);
        CHECK(game.getScore() == 0);
        CHECK(game.getDialValue() == doctest::Approx(SignalLockGame::START_VALUE));
    }
}
