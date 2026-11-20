#include "TankGame.h"

#include <algorithm>
#include <cmath>
#include <numbers>
#include <random>

#include "RippleField.h"

namespace {
float defaultRandom(const float min, const float max)
{
    static std::mt19937 engine{ std::random_device{}() };
    if (max <= min)
        return min;

    return std::uniform_real_distribution<float>(min, max)(engine);
}

constexpr float TICK = 1.0f; // seconds between two steps of the timer
constexpr int SPAWN_ATTEMPTS = 100;
}

TankGame::TankGame(const ax::Vec2& waterSize, Random random)
    : _size(waterSize), _random(random ? std::move(random) : Random(defaultRandom)), _contacts(CONTACT_COUNT)
{
}

void TankGame::reset()
{
    _tick = 0.0f;
    setScore(0);
    setTimer(START_TIME);

    for (auto& contact : _contacts)
        spawn(contact);

    setState(State::START);
}

void TankGame::start()
{
    if (_state != State::START)
        return;

    _tick = 0.0f;
    setState(State::PLAY);
    tickTimer();
}

void TankGame::strike(const ax::Vec2& position)
{
    if (_state != State::PLAY)
        return;

    for (auto& contact : _contacts) {
        if (position.getDistance(contact.position) <= RippleField::REACH) {
            setScore(_score + 1);
            respawnAwayFrom(contact, position);
        }
    }
}

void TankGame::update(const float dt)
{
    if (_state != State::PLAY)
        return;

    for (auto& contact : _contacts)
        moveContact(contact, dt);

    _tick += dt;
    while (_tick >= TICK && _state == State::PLAY) {
        _tick -= TICK;
        tickTimer();
    }
}

void TankGame::setScore(const int score)
{
    _score = score;
    if (onScoreChanged)
        onScoreChanged(score);
}

void TankGame::setTimer(const int timer)
{
    _timer = timer;
    if (onTimerChanged)
        onTimerChanged(timer);
}

void TankGame::setState(const State state)
{
    _state = state;
    if (onStateChanged)
        onStateChanged(state);
}

void TankGame::tickTimer()
{
    if (_timer == 0)
        setState(State::END); // the time is up
    else
        setTimer(_timer - 1);
}

void TankGame::moveContact(Contact& contact, const float dt) const
{
    contact.position += contact.direction * dt;

    // Bounce off the left and right edges, then off the bottom and top ones.
    if (contact.position.x - CONTACT_RADIUS <= 0.0f || contact.position.x + CONTACT_RADIUS >= _size.x)
        contact.direction.x *= -1.0f;

    if (contact.position.y - CONTACT_RADIUS <= 0.0f || contact.position.y + CONTACT_RADIUS >= _size.y)
        contact.direction.y *= -1.0f;
}

void TankGame::spawn(Contact& contact)
{
    contact.position = randomLocation();
    contact.direction = randomDirection();
}

void TankGame::respawnAwayFrom(Contact& contact, const ax::Vec2& position)
{
    for (int attempt = 0; attempt < SPAWN_ATTEMPTS; ++attempt) {
        const ax::Vec2 candidate = randomLocation();
        if (position.getDistance(candidate) > RippleField::REACH) {
            contact.position = candidate;
            contact.direction = randomDirection();
            return;
        }
    }

    // A stubborn random source: use the corner of the water that is farthest from the ripple.
    const ax::Vec2 corners[] = {
        { SPAWN_PADDING, SPAWN_PADDING },
        { _size.x - SPAWN_PADDING, SPAWN_PADDING },
        { SPAWN_PADDING, _size.y - SPAWN_PADDING },
        { _size.x - SPAWN_PADDING, _size.y - SPAWN_PADDING },
    };
    contact.position = *std::max_element(std::begin(corners), std::end(corners),
        [&position](const ax::Vec2& a, const ax::Vec2& b) { return position.getDistance(a) < position.getDistance(b); });
    contact.direction = randomDirection();
}

ax::Vec2 TankGame::randomLocation()
{
    return { _random(SPAWN_PADDING, _size.x - SPAWN_PADDING), _random(SPAWN_PADDING, _size.y - SPAWN_PADDING) };
}

ax::Vec2 TankGame::randomDirection()
{
    const float angle = _random(-std::numbers::pi_v<float>, std::numbers::pi_v<float>);
    return { std::cos(angle) * CONTACT_SPEED, std::sin(angle) * CONTACT_SPEED };
}
