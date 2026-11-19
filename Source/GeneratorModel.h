#pragma once

// The state of one generator: a value between 0 and 1 that glides toward a
// target. The player moves the target in fixed steps; the value follows at a
// fixed rate.
//
// This class does not depend on Axmol, so it can be read and tested on its own.
class GeneratorModel
{
public:
    // `rate` is how much of the full range the value covers per second.
    explicit GeneratorModel(const float rate) : _rate(rate) {}

    // Jump to `value` (clamped to 0..1) without gliding.
    void reset(float value);

    void increase();
    void decrease();

    void update(float dt);

    [[nodiscard]] float getValue() const { return _value; }
    [[nodiscard]] float getTarget() const { return _target; }

private:
    static constexpr float STEP = 0.25f; // how far one click moves the target

    float _rate;
    float _value = 0.0f;
    float _target = 0.0f;
};
