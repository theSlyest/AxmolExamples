#include "doctest/doctest.h"

#include "GeneratorModel.h"

TEST_SUITE("GeneratorModel") {
    TEST_CASE("reset sets the value and the target, clamped to 0..1") {
        GeneratorModel generator(0.5f);

        generator.reset(0.3f);
        CHECK(generator.getValue() == doctest::Approx(0.3f));
        CHECK(generator.getTarget() == doctest::Approx(0.3f));

        generator.reset(2.0f);
        CHECK(generator.getValue() == doctest::Approx(1.0f));

        generator.reset(-1.0f);
        CHECK(generator.getValue() == doctest::Approx(0.0f));
    }

    TEST_CASE("increase and decrease move the target by a quarter, within 0..1") {
        GeneratorModel generator(0.5f);
        generator.reset(0.5f);

        generator.increase();
        CHECK(generator.getTarget() == doctest::Approx(0.75f));
        generator.increase();
        generator.increase();
        CHECK(generator.getTarget() == doctest::Approx(1.0f)); // clamped

        generator.reset(0.5f);
        generator.decrease();
        generator.decrease();
        generator.decrease();
        CHECK(generator.getTarget() == doctest::Approx(0.0f)); // clamped
    }

    TEST_CASE("the value does not move until the target changes") {
        GeneratorModel generator(0.5f);
        generator.reset(0.4f);

        generator.update(10.0f);
        CHECK(generator.getValue() == doctest::Approx(0.4f));
    }

    TEST_CASE("the value glides toward the target at the generator's rate") {
        GeneratorModel generator(0.5f); // half of the range per second
        generator.reset(0.5f);
        generator.increase(); // target 0.75

        generator.update(0.25f);
        CHECK(generator.getValue() == doctest::Approx(0.625f));
        CHECK(generator.getTarget() == doctest::Approx(0.75f));
    }

    TEST_CASE("the value stops on the target without overshooting") {
        GeneratorModel generator(0.5f);
        generator.reset(0.5f);
        generator.increase();
        generator.update(10.0f);
        CHECK(generator.getValue() == doctest::Approx(0.75f));

        generator.decrease();
        generator.decrease();
        generator.update(10.0f);
        CHECK(generator.getValue() == doctest::Approx(0.25f));
    }

    TEST_CASE("a slower generator takes longer to reach the same target") {
        GeneratorModel fast(0.5f);
        GeneratorModel slow(0.125f);
        for (auto* generator : { &fast, &slow }) {
            generator->reset(0.5f);
            generator->increase();
            generator->update(0.5f);
        }

        CHECK(fast.getValue() > slow.getValue());
    }
}
