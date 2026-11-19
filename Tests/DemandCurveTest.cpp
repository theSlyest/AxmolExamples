#include "doctest/doctest.h"

#include <vector>

#include "DemandCurve.h"

namespace {
// A random source that always picks the middle of the range.
float middle(const float min, const float max)
{
    return (min + max) / 2.0f;
}

constexpr float FRAME = 1.0f / 60.0f;
}

TEST_SUITE("DemandCurve") {
    TEST_CASE("reset starts the curve at the initial value") {
        DemandCurve curve(middle);

        curve.reset(0.5f);
        CHECK(curve.getValue() == doctest::Approx(0.5f));

        curve.reset(0.3f);
        CHECK(curve.getValue() == doctest::Approx(0.3f));
    }

    TEST_CASE("the value stays between 0 and 1") {
        DemandCurve curve; // default random source
        curve.reset(0.5f);

        for (int i = 0; i < 60 * 180; ++i) {
            curve.update(FRAME);
            REQUIRE(curve.getValue() >= 0.0f);
            REQUIRE(curve.getValue() <= 1.0f);
        }
    }

    TEST_CASE("the same random source gives the same curve") {
        DemandCurve first(middle);
        DemandCurve second(middle);
        first.reset(0.5f);
        second.reset(0.5f);

        for (int i = 0; i < 60 * 60; ++i) {
            first.update(FRAME);
            second.update(FRAME);
            REQUIRE(first.getValue() == doctest::Approx(second.getValue()));
        }
    }

    TEST_CASE("the curve actually moves") {
        DemandCurve curve(middle);
        curve.reset(0.5f);

        float min = 1.0f;
        float max = 0.0f;
        for (int i = 0; i < 60 * 30; ++i) {
            curve.update(FRAME);
            min = std::min(min, curve.getValue());
            max = std::max(max, curve.getValue());
        }

        CHECK(max - min > 0.2f);
    }

    TEST_CASE("the random source is asked once per peak or trough") {
        int calls = 0;
        DemandCurve curve([&calls](const float min, const float max) {
            ++calls;
            return (min + max) / 2.0f;
        });

        curve.reset(0.5f);
        CHECK(calls == 1); // the first amplitude

        for (int i = 0; i < 60 * 30; ++i)
            curve.update(FRAME);
        CHECK(calls > 1);
    }

    TEST_CASE("peaks and troughs get closer together as time passes") {
        int calls = 0;
        DemandCurve curve([&calls](const float min, const float max) {
            ++calls;
            return (min + max) / 2.0f;
        });
        curve.reset(0.5f);

        int early = 0;
        int late = 0;
        for (int i = 0; i < 60 * 120; ++i) {
            const int before = calls;
            curve.update(FRAME);
            if (i < 60 * 30)
                early += calls - before;
            else if (i >= 60 * 90)
                late += calls - before;
        }

        CHECK(late > early);
    }
}
