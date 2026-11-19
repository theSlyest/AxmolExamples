#pragma once

#include <functional>

// The demand the player has to follow: a sine wave whose amplitude and offset
// are re-randomized at every peak and trough, and whose period shrinks over
// time to make the game harder.
//
// This class does not depend on Axmol, so it can be read and tested on its
// own. Values are normalized between 0 and 1.
class DemandCurve
{
public:
    // Returns a random number between min and max (inclusive).
    using Random = std::function<float(float min, float max)>;

    // Uses a standard random engine when no source is given.
    explicit DemandCurve(Random random = nullptr);

    // Restart the curve around `initialValue` with the slowest period.
    void reset(float initialValue);

    void update(float dt);

    [[nodiscard]] float getValue() const { return _value; }

private:
    static constexpr float PERIOD_DELAY = 3.0f; // seconds between two period decreases
    static constexpr float PERIOD_STEP = 0.25f;
    static constexpr float PERIOD_MIN = 6.0f;
    static constexpr float PERIOD_START = 12.0f;
    static constexpr float MIN_AMP = 0.1f;
    static constexpr float MAX_AMP = 0.5f;

    Random _random;

    float _value = 0.5f;
    float _midLine = 0.5f;
    float _amplitude = 0.0f;
    float _period = PERIOD_START;
    float _periodTimer = 0.0f;
    float _phase = 0.0f;
    int _sinSign = 1;

    void onExtremumReached(bool isPeak);
};
