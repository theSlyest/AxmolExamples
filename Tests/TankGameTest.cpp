#include "doctest/doctest.h"

#include <cmath>
#include <numbers>
#include <vector>

#include "RippleField.h"
#include "TankGame.h"

namespace {
const ax::Vec2 WATER(952.0f, 952.0f);
constexpr float HALF_PI = std::numbers::pi_v<float> / 2.0f;

// A random source that returns the given numbers in order, then the middle of each range.
struct Script
{
    std::vector<float> values;
    size_t next = 0;

    float operator()(const float min, const float max)
    {
        if (next < values.size())
            return values[next++];

        return (min + max) / 2.0f;
    }
};

// Every contact in the middle of the water, drifting to the right.
TankGame::Random middle()
{
    return [](const float min, const float max) { return (min + max) / 2.0f; };
}

// `positions` and `angles` give each contact's starting point and direction.
Script contactsAt(const std::vector<ax::Vec2>& positions, const std::vector<float>& angles)
{
    Script script;
    for (size_t i = 0; i < positions.size(); ++i) {
        script.values.push_back(positions[i].x);
        script.values.push_back(positions[i].y);
        script.values.push_back(angles[i]);
    }
    return script;
}

// Run the round for `seconds`, half a second at a time.
void run(TankGame& game, const float seconds)
{
    for (float t = 0.0f; t < seconds; t += 0.5f)
        game.update(0.5f);
}
}

TEST_SUITE("TankGame start") {
    TEST_CASE("a new round has no score, a full timer and the contacts on the water") {
        TankGame game(WATER);
        game.reset();

        CHECK(game.getState() == TankGame::State::START);
        CHECK(game.getScore() == 0);
        CHECK(game.getTimer() == TankGame::START_TIME);
        REQUIRE(game.getContacts().size() == TankGame::CONTACT_COUNT);
        for (const auto& contact : game.getContacts()) {
            CHECK(contact.position.x >= TankGame::SPAWN_PADDING);
            CHECK(contact.position.x <= WATER.x - TankGame::SPAWN_PADDING);
            CHECK(contact.position.y >= TankGame::SPAWN_PADDING);
            CHECK(contact.position.y <= WATER.y - TankGame::SPAWN_PADDING);
        }
    }

    TEST_CASE("each contact drifts at CONTACT_SPEED in a random direction") {
        TankGame game(WATER);
        game.reset();

        for (const auto& contact : game.getContacts())
            CHECK(contact.direction.length() == doctest::Approx(TankGame::CONTACT_SPEED));
    }

    TEST_CASE("reset tells the view about the starting state") {
        TankGame game(WATER);
        int score = -1;
        int timer = -1;
        TankGame::State state = TankGame::State::END;
        game.onScoreChanged = [&](const int v) { score = v; };
        game.onTimerChanged = [&](const int v) { timer = v; };
        game.onStateChanged = [&](const TankGame::State v) { state = v; };

        game.reset();

        CHECK(score == 0);
        CHECK(timer == TankGame::START_TIME);
        CHECK(state == TankGame::State::START);
    }

    TEST_CASE("nothing moves and the clock waits until the round starts") {
        TankGame game(WATER, middle());
        game.reset();
        const ax::Vec2 before = game.getContacts().front().position;

        game.update(5.0f);

        CHECK(game.getState() == TankGame::State::START);
        CHECK(game.getTimer() == TankGame::START_TIME);
        CHECK(game.getContacts().front().position == before);
    }

    TEST_CASE("starting begins the round and takes the first second off the timer") {
        TankGame game(WATER);
        game.reset();
        TankGame::State state = TankGame::State::START;
        game.onStateChanged = [&](const TankGame::State v) { state = v; };

        game.start();

        CHECK(game.getState() == TankGame::State::PLAY);
        CHECK(state == TankGame::State::PLAY);
        CHECK(game.getTimer() == TankGame::START_TIME - 1);
    }

    TEST_CASE("starting again does nothing") {
        TankGame game(WATER);
        game.reset();
        game.start();
        game.update(3.0f);
        const int timer = game.getTimer();

        game.start();

        CHECK(game.getTimer() == timer);
    }
}

TEST_SUITE("TankGame contacts") {
    TEST_CASE("contacts drift in their direction") {
        TankGame game(WATER, middle()); // all in the middle, heading right
        game.reset();
        game.start();
        const ax::Vec2 start = game.getContacts().front().position;

        game.update(2.0f);

        const ax::Vec2 now = game.getContacts().front().position;
        CHECK(now.x == doctest::Approx(start.x + 2.0f * TankGame::CONTACT_SPEED));
        CHECK(now.y == doctest::Approx(start.y));
    }

    TEST_CASE("a contact bounces off the right edge") {
        TankGame game(WATER, middle());
        game.reset();
        game.start();

        run(game, 30.0f); // the right edge is about 29 seconds away

        const auto& contact = game.getContacts().front();
        CHECK(contact.direction.x < 0.0f);
        CHECK(contact.position.x + TankGame::CONTACT_RADIUS <= WATER.x + TankGame::CONTACT_SPEED); // came back
    }

    TEST_CASE("a contact bounces off the top edge") {
        const std::vector<ax::Vec2> positions(4, ax::Vec2(476.0f, 476.0f));
        Script script = contactsAt(positions, { HALF_PI, HALF_PI, HALF_PI, HALF_PI }); // heading up
        TankGame game(WATER, [&script](const float a, const float b) { return script(a, b); });
        game.reset();
        game.start();

        run(game, 30.0f);

        CHECK(game.getContacts().front().direction.y < 0.0f);
        CHECK(game.getContacts().front().direction.x == doctest::Approx(0.0f).epsilon(0.01));
    }

    TEST_CASE("a contact bounces off the left and bottom edges too") {
        const std::vector<ax::Vec2> positions(4, ax::Vec2(30.0f, 30.0f));
        const float toLowerLeft = -std::numbers::pi_v<float> * 3.0f / 4.0f; // down and to the left
        Script script = contactsAt(positions, { toLowerLeft, toLowerLeft, toLowerLeft, toLowerLeft });
        TankGame game(WATER, [&script](const float a, const float b) { return script(a, b); });
        game.reset();
        game.start();

        run(game, 3.0f);

        CHECK(game.getContacts().front().direction.x > 0.0f);
        CHECK(game.getContacts().front().direction.y > 0.0f);
    }
}

TEST_SUITE("TankGame ripples") {
    TEST_CASE("a ripple scores every contact it reaches and sends them elsewhere") {
        Script script = contactsAt(
            { ax::Vec2(100.0f, 100.0f), ax::Vec2(700.0f, 700.0f), ax::Vec2(800.0f, 100.0f), ax::Vec2(100.0f, 800.0f) },
            { 0.0f, 0.0f, 0.0f, 0.0f });
        // The respawn of the first contact: one spot inside the ripple's reach (refused), then a free one, then a direction.
        script.values.insert(script.values.end(), { 120.0f, 120.0f, 600.0f, 600.0f, 1.0f });
        int score = -1;
        TankGame game(WATER, [&script](const float a, const float b) { return script(a, b); });
        game.onScoreChanged = [&](const int v) { score = v; };
        game.reset();
        game.start();

        game.strike(ax::Vec2(110.0f, 110.0f));

        CHECK(game.getScore() == 1);
        CHECK(score == 1);
        CHECK(game.getContacts()[0].position == ax::Vec2(600.0f, 600.0f)); // the refused spot was skipped
        CHECK(game.getContacts()[0].direction.x == doctest::Approx(std::cos(1.0f) * TankGame::CONTACT_SPEED));
        CHECK(game.getContacts()[1].position == ax::Vec2(700.0f, 700.0f)); // the others stayed put
    }

    TEST_CASE("a ripple reaches contacts up to REACH away, and no further") {
        Script script = contactsAt(
            { ax::Vec2(300.0f, 300.0f), ax::Vec2(300.0f + RippleField::REACH, 300.0f),
              ax::Vec2(300.0f + RippleField::REACH + 1.0f, 300.0f), ax::Vec2(800.0f, 800.0f) },
            { 0.0f, 0.0f, 0.0f, 0.0f });
        TankGame game(WATER, [&script](const float a, const float b) { return script(a, b); });
        game.reset();
        game.start();

        game.strike(ax::Vec2(300.0f, 300.0f));

        CHECK(game.getScore() == 2); // the contact under the ripple and the one exactly at REACH
    }

    TEST_CASE("each contact reached scores its own point") {
        TankGame game(WATER, middle()); // all four in the middle
        game.reset();
        game.start();

        game.strike(ax::Vec2(476.0f, 476.0f));

        CHECK(game.getScore() == TankGame::CONTACT_COUNT);
    }

    TEST_CASE("a contact is never respawned inside the ripple's reach, even with a stubborn random source") {
        TankGame game(WATER, middle()); // always proposes the middle of the water
        game.reset();
        game.start();
        const ax::Vec2 strike(576.0f, 476.0f); // close to the middle

        game.strike(strike);

        for (const auto& contact : game.getContacts()) {
            CHECK(strike.getDistance(contact.position) > RippleField::REACH);
            CHECK(contact.position.x >= 0.0f);
            CHECK(contact.position.x <= WATER.x);
            CHECK(contact.position.y >= 0.0f);
            CHECK(contact.position.y <= WATER.y);
        }
    }

    TEST_CASE("a ripple that reaches nothing scores nothing") {
        TankGame game(WATER, middle());
        game.reset();
        game.start();

        game.strike(ax::Vec2(20.0f, 20.0f)); // far from the middle

        CHECK(game.getScore() == 0);
    }

    TEST_CASE("ripples do nothing before the round has started or after it ended") {
        TankGame game(WATER, middle());
        game.reset();

        game.strike(ax::Vec2(476.0f, 476.0f));
        CHECK(game.getScore() == 0);

        game.start();
        run(game, TankGame::START_TIME);
        REQUIRE(game.getState() == TankGame::State::END);
        game.strike(ax::Vec2(476.0f, 476.0f));
        CHECK(game.getScore() == 0);
    }
}

TEST_SUITE("TankGame timer") {
    TEST_CASE("the timer goes down one second at a time") {
        TankGame game(WATER);
        int timer = -1;
        game.onTimerChanged = [&](const int v) { timer = v; };
        game.reset();
        game.start();
        REQUIRE(game.getTimer() == TankGame::START_TIME - 1);

        game.update(0.9f);
        CHECK(game.getTimer() == TankGame::START_TIME - 1);

        game.update(0.2f);
        CHECK(game.getTimer() == TankGame::START_TIME - 2);
        CHECK(timer == TankGame::START_TIME - 2);
    }

    TEST_CASE("the round ends 45 seconds after the first touch, once") {
        TankGame game(WATER);
        int ends = 0;
        game.onStateChanged = [&](const TankGame::State v) { ends += v == TankGame::State::END ? 1 : 0; };
        game.reset();
        game.start();

        run(game, 44.5f);
        CHECK(game.getState() == TankGame::State::PLAY);
        CHECK(game.getTimer() == 0);

        run(game, 1.0f);
        CHECK(game.getState() == TankGame::State::END);
        CHECK(ends == 1);

        run(game, 10.0f);
        CHECK(ends == 1);
    }

    TEST_CASE("the contacts stop and the score stays once the round has ended") {
        TankGame game(WATER, middle());
        game.reset();
        game.start();
        game.strike(ax::Vec2(476.0f, 476.0f));
        run(game, TankGame::START_TIME);
        REQUIRE(game.getState() == TankGame::State::END);
        const int score = game.getScore();
        const ax::Vec2 position = game.getContacts().front().position;

        run(game, 5.0f);

        CHECK(game.getScore() == score);
        CHECK(game.getContacts().front().position == position);
    }

    TEST_CASE("reset starts a new round after the end") {
        TankGame game(WATER);
        game.reset();
        game.start();
        run(game, TankGame::START_TIME);
        REQUIRE(game.getState() == TankGame::State::END);

        game.reset();

        CHECK(game.getState() == TankGame::State::START);
        CHECK(game.getScore() == 0);
        CHECK(game.getTimer() == TankGame::START_TIME);
    }

    TEST_CASE("reset in the middle of a round starts over") {
        TankGame game(WATER, middle());
        game.reset();
        game.start();
        game.strike(ax::Vec2(476.0f, 476.0f));
        run(game, 10.0f);

        game.reset();

        CHECK(game.getState() == TankGame::State::START);
        CHECK(game.getScore() == 0);
        CHECK(game.getTimer() == TankGame::START_TIME);
    }
}
