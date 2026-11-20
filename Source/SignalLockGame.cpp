#include "SignalLockGame.h"

#include <algorithm>
#include <cmath>
#include <random>

namespace {
float defaultRandom()
{
    static std::mt19937 engine{ std::random_device{}() };
    return std::uniform_real_distribution<float>(0.0f, 1.0f)(engine);
}
}

SignalLockGame::SignalLockGame(Random random) : _random(random ? std::move(random) : Random(defaultRandom))
{
}

void SignalLockGame::reset()
{
    _playing = false;
    _over = false;
    _dial = START_VALUE;
    _station = 0.0f;
    _lockTime = 0.0f;
    _glitch = 0.0f;
    _timer = 0.0f;
    setScore(0);
    setTime(START_TIME);
}

void SignalLockGame::update(const float dt, const int direction)
{
    if (_over)
        return;

    if (!_playing) {
        if (direction == 0)
            return;

        _playing = true;
        newStation();
    }

    _timer += dt;
    if (_timer >= 1.0f) {
        _timer -= 1.0f;
        setTime(_time - 1);

        if (_time == 0) {
            endRound();
            return;
        }
    }

    _dial = std::clamp(_dial + static_cast<float>(direction) * TUNE_SPEED * dt, 0.0f, 1.0f);

    const float dist = std::abs(_dial - _station);
    _glitch = std::min(1.0f, dist / DETECT_DIST);

    if (dist <= LOCK_DIST) {
        _lockTime += dt;

        if (_lockTime >= LOCK_DELAY) {
            setScore(_score + 1);
            _lockTime = 0.0f;
            newStation();
        }
    }
    else {
        _lockTime = 0.0f;
    }
}

void SignalLockGame::setScore(const int score)
{
    _score = score;
    if (onScoreChanged)
        onScoreChanged(score);
}

void SignalLockGame::setTime(const int time)
{
    _time = time;
    if (onTimeChanged)
        onTimeChanged(time);
}

void SignalLockGame::newStation()
{
    // Pick a station away from the dial. Give up on a stubborn random source and
    // use the far end of the dial, which is always far enough.
    for (int attempt = 0; attempt < 100; ++attempt) {
        _station = std::clamp(_random(), 0.0f, 1.0f);
        if (std::abs(_station - _dial) >= LOCK_SAFE)
            return;
    }

    _station = _dial < 0.5f ? 1.0f : 0.0f;
}

void SignalLockGame::endRound()
{
    _playing = false;
    _over = true;
    if (onGameOver)
        onGameOver();
}
