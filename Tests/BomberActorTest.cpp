#include "doctest/doctest.h"

#include "BomberPlayer.h"
#include "Directions.h"

namespace {
// The actors are created without init(): init() builds the drawing, which
// needs the renderer (and so a window). The movement and timer logic under
// test does not use it.
struct PlayerFixture
{
    BomberPlayer* player = new BomberPlayer();

    ~PlayerFixture() { player->release(); }
};
}

TEST_SUITE("BomberActor") {
    TEST_CASE("an idle actor has not reached any destination") {
        PlayerFixture f;
        f.player->setDirection(ax::Vec2::ZERO);
        f.player->setDestination(ax::Vec2(100.0f, 100.0f));

        CHECK_FALSE(f.player->hasReachedDestination(ax::Vec2(100.0f, 100.0f)));
        CHECK_FALSE(f.player->hasReachedDestination(ax::Vec2(500.0f, 500.0f)));
    }

    TEST_CASE("an actor moving right has arrived once x reaches the destination") {
        PlayerFixture f;
        f.player->setDirection(DIR_RIGHT);
        f.player->setDestination(ax::Vec2(96.0f, 48.0f));

        CHECK_FALSE(f.player->hasReachedDestination(ax::Vec2(95.0f, 48.0f)));
        CHECK(f.player->hasReachedDestination(ax::Vec2(96.0f, 48.0f)));
        CHECK(f.player->hasReachedDestination(ax::Vec2(100.0f, 48.0f))); // passed it
    }

    TEST_CASE("an actor moving left has arrived once x reaches the destination") {
        PlayerFixture f;
        f.player->setDirection(DIR_LEFT);
        f.player->setDestination(ax::Vec2(48.0f, 48.0f));

        CHECK_FALSE(f.player->hasReachedDestination(ax::Vec2(49.0f, 48.0f)));
        CHECK(f.player->hasReachedDestination(ax::Vec2(48.0f, 48.0f)));
        CHECK(f.player->hasReachedDestination(ax::Vec2(40.0f, 48.0f)));
    }

    TEST_CASE("an actor moving up has arrived once y reaches the destination") {
        PlayerFixture f;
        f.player->setDirection(DIR_UP);
        f.player->setDestination(ax::Vec2(48.0f, 96.0f));

        CHECK_FALSE(f.player->hasReachedDestination(ax::Vec2(48.0f, 95.0f)));
        CHECK(f.player->hasReachedDestination(ax::Vec2(48.0f, 96.0f)));
        CHECK(f.player->hasReachedDestination(ax::Vec2(48.0f, 120.0f)));
    }

    TEST_CASE("an actor moving down has arrived once y reaches the destination") {
        PlayerFixture f;
        f.player->setDirection(DIR_DOWN);
        f.player->setDestination(ax::Vec2(48.0f, 48.0f));

        CHECK_FALSE(f.player->hasReachedDestination(ax::Vec2(48.0f, 49.0f)));
        CHECK(f.player->hasReachedDestination(ax::Vec2(48.0f, 48.0f)));
        CHECK(f.player->hasReachedDestination(ax::Vec2(48.0f, 10.0f)));
    }

    TEST_CASE("only the axis of travel matters") {
        PlayerFixture f;
        f.player->setDirection(DIR_RIGHT);
        f.player->setDestination(ax::Vec2(96.0f, 48.0f));

        CHECK_FALSE(f.player->hasReachedDestination(ax::Vec2(90.0f, 500.0f)));
    }

    TEST_CASE("the direction and destination are stored") {
        PlayerFixture f;
        f.player->setDirection(DIR_UP);
        f.player->setDestination(ax::Vec2(12.0f, 34.0f));

        CHECK(f.player->getDirection() == DIR_UP);
        CHECK(f.player->getDestination() == ax::Vec2(12.0f, 34.0f));
    }

    TEST_CASE("spawn invincibility lasts SPAWN_DELAY seconds") {
        PlayerFixture f;
        CHECK_FALSE(f.player->isInvincible());

        f.player->setInvincible(true);
        CHECK(f.player->isInvincible());

        f.player->update(BomberActor::SPAWN_DELAY / 2.0f);
        CHECK(f.player->isInvincible());

        f.player->update(BomberActor::SPAWN_DELAY / 2.0f + 0.01f);
        CHECK_FALSE(f.player->isInvincible());
    }

    TEST_CASE("invincibility can be cancelled") {
        PlayerFixture f;
        f.player->setInvincible(true);

        f.player->setInvincible(false);

        CHECK_FALSE(f.player->isInvincible());
    }
}

TEST_SUITE("BomberPlayer") {
    TEST_CASE("the player starts without a bomb and can drop and recover one") {
        PlayerFixture f;
        CHECK_FALSE(f.player->hasBomb());

        f.player->dropBomb();
        CHECK(f.player->hasBomb());

        f.player->clearBomb();
        CHECK_FALSE(f.player->hasBomb());
    }
}
