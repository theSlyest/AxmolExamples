#include "DemandCurve.h"

#include <algorithm>
#include <cmath>
#include <numbers>
#include <random>

namespace {
    float defaultRandom(const float min, const float max)
    {
        static std::mt19937 engine{ std::random_device{}() };
        if (max <= min)
            return min;

        return std::uniform_real_distribution<float>(min, max)(engine);
    }
}

DemandCurve::DemandCurve(Random random) : _random(random ? std::move(random) : Random(defaultRandom))
{
}

void DemandCurve::reset(const float initialValue)
{
    _value = initialValue;
    _midLine = initialValue;
    _amplitude = 0.0f;
    _period = PERIOD_START;
    _sinSign = -1;
    _phase = 1.5f * std::numbers::pi_v<float>;
    onExtremumReached(false);
}

void DemandCurve::update(const float dt)
{
    _periodTimer += dt;
    if (_periodTimer >= PERIOD_DELAY) {
        _periodTimer -= PERIOD_DELAY;
        _period = std::max(PERIOD_MIN, _period - PERIOD_STEP);
    }

    constexpr float twoPi = 2.0f * std::numbers::pi_v<float>;
    _phase += dt * twoPi / _period;
    if (_phase >= twoPi)
        _phase = std::fmod(_phase, twoPi);

    const float sinVal = std::sin(_phase);

    // The cosine changes sign at every peak and trough of the sine.
    if (const int cosSign = (std::cos(_phase) >= 0.0f) ? 1 : -1; cosSign != _sinSign) {
        onExtremumReached(sinVal > 0.0f);
        _sinSign = cosSign;
    }

    _value = std::clamp(_midLine + sinVal * _amplitude / 2.0f, 0.0f, 1.0f);
}

void DemandCurve::onExtremumReached(const bool isPeak)
{
    const float halfAmp = _amplitude / 2.0f;
    if (isPeak) {
        const float extremumY = _midLine + halfAmp;
        _amplitude = _random(std::min(MIN_AMP, extremumY), std::min(MAX_AMP, extremumY));
        _midLine = extremumY - _amplitude / 2.0f;
    }
    else {
        const float extremumY = _midLine - halfAmp;
        const float maxAmp = 1.0f - extremumY;
        _amplitude = _random(std::min(MIN_AMP, maxAmp), std::min(MAX_AMP, maxAmp));
        _midLine = extremumY + _amplitude / 2.0f;
    }
}
