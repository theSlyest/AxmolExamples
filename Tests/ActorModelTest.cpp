#include "doctest/doctest.h"

#include "ActorModel.h"
#include "Directions.h"

TEST_SUITE("ActorModel") {
    TEST_CASE("the position, direction, destination and speed are stored") {
        ActorModel actor(120.0f);
        actor.setPosition({ 48.0f, 96.0f });
        actor.setDirection(DIR_UP);
        actor.setDestination({ 48.0f, 144.0f });

        CHECK(actor.getPosition() == ax::Vec2(48.0f, 96.0f));
        CHECK(actor.getDirection() == DIR_UP);
        CHECK(actor.getDestination() == ax::Vec2(48.0f, 144.0f));
        CHECK(actor.getSpeed() == doctest::Approx(120.0f));
    }

    TEST_CASE("an idle actor has not reached any destination") {
        ActorModel actor;
        actor.setDirection(ax::Vec2::ZERO);
        actor.setDestination({ 100.0f, 100.0f });

        CHECK_FALSE(actor.hasReachedDestination({ 100.0f, 100.0f }));
        CHECK_FALSE(actor.hasReachedDestination({ 500.0f, 500.0f }));
    }

    TEST_CASE("an actor moving right has arrived once x reaches the destination") {
        ActorModel actor;
        actor.setDirection(DIR_RIGHT);
        actor.setDestination({ 96.0f, 48.0f });

        CHECK_FALSE(actor.hasReachedDestination({ 95.0f, 48.0f }));
        CHECK(actor.hasReachedDestination({ 96.0f, 48.0f }));
        CHECK(actor.hasReachedDestination({ 100.0f, 48.0f })); // passed it
    }

    TEST_CASE("an actor moving left has arrived once x reaches the destination") {
        ActorModel actor;
        actor.setDirection(DIR_LEFT);
        actor.setDestination({ 48.0f, 48.0f });

        CHECK_FALSE(actor.hasReachedDestination({ 49.0f, 48.0f }));
        CHECK(actor.hasReachedDestination({ 48.0f, 48.0f }));
        CHECK(actor.hasReachedDestination({ 40.0f, 48.0f }));
    }

    TEST_CASE("an actor moving up has arrived once y reaches the destination") {
        ActorModel actor;
        actor.setDirection(DIR_UP);
        actor.setDestination({ 48.0f, 96.0f });

        CHECK_FALSE(actor.hasReachedDestination({ 48.0f, 95.0f }));
        CHECK(actor.hasReachedDestination({ 48.0f, 96.0f }));
        CHECK(actor.hasReachedDestination({ 48.0f, 120.0f }));
    }

    TEST_CASE("an actor moving down has arrived once y reaches the destination") {
        ActorModel actor;
        actor.setDirection(DIR_DOWN);
        actor.setDestination({ 48.0f, 48.0f });

        CHECK_FALSE(actor.hasReachedDestination({ 48.0f, 49.0f }));
        CHECK(actor.hasReachedDestination({ 48.0f, 48.0f }));
        CHECK(actor.hasReachedDestination({ 48.0f, 10.0f }));
    }

    TEST_CASE("only the axis of travel matters") {
        ActorModel actor;
        actor.setDirection(DIR_RIGHT);
        actor.setDestination({ 96.0f, 48.0f });

        CHECK_FALSE(actor.hasReachedDestination({ 90.0f, 500.0f }));
    }

    TEST_CASE("spawn invincibility lasts SPAWN_DELAY seconds") {
        ActorModel actor;
        CHECK_FALSE(actor.isInvincible());

        actor.setInvincible(true);
        CHECK(actor.isInvincible());

        actor.update(ActorModel::SPAWN_DELAY / 2.0f);
        CHECK(actor.isInvincible());

        actor.update(ActorModel::SPAWN_DELAY / 2.0f + 0.01f);
        CHECK_FALSE(actor.isInvincible());
    }

    TEST_CASE("invincibility can be cancelled") {
        ActorModel actor;
        actor.setInvincible(true);

        actor.setInvincible(false);

        CHECK_FALSE(actor.isInvincible());
    }
}
