#include "doctest/doctest.h"

#include <cmath>

#include "RippleField.h"

namespace {
constexpr float ORIGIN_X = 400.0f;
constexpr float ORIGIN_Y = 300.0f;

const ax::Vec2 ORIGIN(ORIGIN_X, ORIGIN_Y);

// The height at a point `distance` pixels from the origin, along the x axis.
float heightAt(const RippleField& field, const float distance)
{
    return field.heightAt(ORIGIN_X + distance, ORIGIN_Y);
}
}

TEST_SUITE("RippleField ripples") {
    TEST_CASE("calm water has no ripples") {
        RippleField field;

        CHECK(field.isEmpty());
        CHECK(field.getRipples().empty());
    }

    TEST_CASE("a new ripple starts at its origin with no radius and full height") {
        RippleField field;

        field.add(ORIGIN);

        REQUIRE(field.getRipples().size() == 1);
        const auto& ripple = field.getRipples().front();
        CHECK(ripple.origin == ORIGIN);
        CHECK(ripple.radius == doctest::Approx(0.0f));
        CHECK(ripple.amplitude == doctest::Approx(RippleField::AMPLITUDE));
        CHECK_FALSE(field.isEmpty());
    }

    TEST_CASE("a ripple spreads at SPEED") {
        RippleField field;
        field.add(ORIGIN);

        field.advance(0.2f);

        CHECK(field.getRipples().front().radius == doctest::Approx(0.2f * RippleField::SPEED));
    }

    TEST_CASE("a ripple fades in proportion to its age") {
        RippleField field;
        field.add(ORIGIN);

        field.advance(RippleField::LIFESPAN / 2.0f);
        CHECK(field.getRipples().front().amplitude == doctest::Approx(RippleField::AMPLITUDE / 2.0f));

        field.advance(RippleField::LIFESPAN / 4.0f);
        CHECK(field.getRipples().front().amplitude == doctest::Approx(RippleField::AMPLITUDE / 4.0f));
    }

    TEST_CASE("a ripple is gone after its lifespan") {
        RippleField field;
        field.add(ORIGIN);

        field.advance(RippleField::LIFESPAN - 0.01f);
        CHECK_FALSE(field.isEmpty());

        field.advance(0.02f);
        CHECK(field.isEmpty());
    }

    TEST_CASE("a ripple travels REACH pixels over its lifespan") {
        CHECK(RippleField::REACH == doctest::Approx(RippleField::SPEED * RippleField::LIFESPAN));

        RippleField field;
        field.add(ORIGIN);
        field.advance(RippleField::LIFESPAN - 0.001f);

        CHECK(field.getRipples().front().radius == doctest::Approx(RippleField::REACH).epsilon(0.01));
    }

    TEST_CASE("ripples age separately, and only the old one goes") {
        RippleField field;
        field.add(ORIGIN);
        field.advance(0.6f);
        field.add(ax::Vec2(100.0f, 100.0f));

        field.advance(0.5f); // the first is now 1.1 s old, the second 0.5 s

        REQUIRE(field.getRipples().size() == 1);
        CHECK(field.getRipples().front().origin == ax::Vec2(100.0f, 100.0f));
        CHECK(field.getRipples().front().amplitude == doctest::Approx(RippleField::AMPLITUDE * 0.5f));
    }

    TEST_CASE("clear calms the water") {
        RippleField field;
        field.add(ORIGIN);
        field.add(ax::Vec2(10.0f, 10.0f));

        field.clear();

        CHECK(field.isEmpty());
        CHECK(field.heightAt(ORIGIN_X + 20.0f, ORIGIN_Y) == doctest::Approx(0.0f));
    }
}

TEST_SUITE("RippleField heights") {
    TEST_CASE("calm water is flat everywhere") {
        const RippleField field;

        CHECK(field.heightAt(0.0f, 0.0f) == doctest::Approx(0.0f));
        CHECK(field.heightAt(ORIGIN_X, ORIGIN_Y) == doctest::Approx(0.0f));
    }

    TEST_CASE("the origin itself does not move") {
        RippleField field;
        field.add(ORIGIN);
        field.advance(0.01f);

        CHECK(field.heightAt(ORIGIN_X, ORIGIN_Y) == doctest::Approx(0.0f));
    }

    TEST_CASE("the surface is highest at the wave front, as high as the ripple's amplitude") {
        RippleField field;
        field.add(ORIGIN);
        field.advance(0.4f);

        const auto& ripple = field.getRipples().front();

        // On the front the wave is at its peak and not attenuated.
        CHECK(heightAt(field, ripple.radius) == doctest::Approx(ripple.amplitude));
        CHECK(ripple.amplitude == doctest::Approx(RippleField::AMPLITUDE * 0.6f));
    }

    TEST_CASE("the wave is the same in every direction") {
        RippleField field;
        field.add(ORIGIN);
        field.advance(0.4f);
        const float radius = field.getRipples().front().radius;
        const float amplitude = field.getRipples().front().amplitude;

        CHECK(field.heightAt(ORIGIN_X - radius, ORIGIN_Y) == doctest::Approx(amplitude));
        CHECK(field.heightAt(ORIGIN_X, ORIGIN_Y + radius) == doctest::Approx(amplitude));
        CHECK(field.heightAt(ORIGIN_X, ORIGIN_Y - radius) == doctest::Approx(amplitude));

        const float diagonal = radius / std::sqrt(2.0f);
        CHECK(field.heightAt(ORIGIN_X + diagonal, ORIGIN_Y + diagonal) == doctest::Approx(amplitude));
    }

    TEST_CASE("the wave dies away from the front") {
        RippleField field;
        field.add(ORIGIN);
        field.advance(0.4f);
        const float radius = field.getRipples().front().radius;

        const float onFront = std::fabs(heightAt(field, radius));
        const float nearFront = std::fabs(heightAt(field, radius + 20.0f));
        const float awayFromFront = std::fabs(heightAt(field, radius + 60.0f));

        CHECK(nearFront < onFront);
        CHECK(awayFromFront < nearFront);
    }

    TEST_CASE("points far from the front are not moved at all") {
        RippleField field;
        field.add(ORIGIN);
        field.advance(0.4f);
        const float radius = field.getRipples().front().radius;

        CHECK(heightAt(field, radius + 130.0f) == doctest::Approx(0.0f));
        CHECK(heightAt(field, radius + 60.0f) != doctest::Approx(0.0f).epsilon(1e-6)); // still inside the window
    }

    TEST_CASE("a fading ripple gives a lower wave") {
        RippleField young;
        young.add(ORIGIN);
        young.advance(0.2f);

        RippleField old;
        old.add(ORIGIN);
        old.advance(0.8f);

        const float youngPeak = heightAt(young, young.getRipples().front().radius);
        const float oldPeak = heightAt(old, old.getRipples().front().radius);

        CHECK(oldPeak < youngPeak);
    }

    TEST_CASE("overlapping ripples add up") {
        // Two ripples of different ages, on the same water.
        RippleField both;
        both.add(ORIGIN);
        both.advance(0.2f);
        both.add(ax::Vec2(ORIGIN_X + 60.0f, ORIGIN_Y));
        both.advance(0.2f);

        // The same two ripples, each on its own water.
        RippleField first;
        first.add(ORIGIN);
        first.advance(0.2f);
        first.advance(0.2f);

        RippleField second;
        second.add(ax::Vec2(ORIGIN_X + 60.0f, ORIGIN_Y));
        second.advance(0.2f);

        for (const float x : { ORIGIN_X - 30.0f, ORIGIN_X + 10.0f, ORIGIN_X + 40.0f, ORIGIN_X + 75.0f, ORIGIN_X + 110.0f })
            CHECK(both.heightAt(x, ORIGIN_Y) == doctest::Approx(first.heightAt(x, ORIGIN_Y) + second.heightAt(x, ORIGIN_Y)));
    }

    TEST_CASE("a new ripple does not cut off an older one") {
        RippleField field;
        field.add(ORIGIN);
        field.advance(0.4f);
        const float before = heightAt(field, field.getRipples().front().radius);

        field.add(ax::Vec2(ORIGIN_X + 200.0f, ORIGIN_Y)); // far away, brand new
        field.advance(0.0f);

        CHECK(heightAt(field, field.getRipples().front().radius) == doctest::Approx(before).epsilon(0.05));
    }
}
