#pragma once

#include <functional>
#include <vector>

#include "math/Vec2.h"

// The rules of the game: contacts drift across the water, a ripple sent from a
// click scores every contact it reaches, and the round lasts 45 seconds.
//
// This class has no nodes and no drawing, so it can be read and tested on its
// own. Positions are in pixels on the water, with the origin at its corner. The
// scene draws the contacts and listens to the callbacks.
class TankGame
{
public:
    enum class State { START, PLAY, END };

    static constexpr int START_TIME = 45;          // seconds per round
    static constexpr int CONTACT_COUNT = 4;
    static constexpr float CONTACT_RADIUS = 7.0f;
    static constexpr float CONTACT_SPEED = 16.0f;  // pixels per second
    static constexpr float SPAWN_PADDING = 16.0f;  // contacts appear at least this far from the edges

    struct Contact
    {
        ax::Vec2 position;
        ax::Vec2 direction; // pixels per second
    };

    // Returns a random number between min and max. Uses a standard random engine when no source is given.
    using Random = std::function<float(float min, float max)>;

    TankGame(const ax::Vec2& waterSize, Random random = nullptr);

    // Notifications for the view.
    std::function<void(int)> onScoreChanged;
    std::function<void(int)> onTimerChanged;
    std::function<void(State)> onStateChanged;

    // Get ready for a new round: no score, a full timer, and the contacts scattered.
    void reset();

    // The first touch starts the round. Ignored once it has started.
    void start();

    // A ripple was sent from `position`: every contact it reaches scores a point
    // and reappears somewhere else.
    void strike(const ax::Vec2& position);

    // Move the contacts and count the seconds. Does nothing outside a round.
    void update(float dt);

    [[nodiscard]] State getState() const { return _state; }
    [[nodiscard]] int getScore() const { return _score; }
    [[nodiscard]] int getTimer() const { return _timer; }
    [[nodiscard]] const std::vector<Contact>& getContacts() const { return _contacts; }

private:
    ax::Vec2 _size;
    Random _random;

    State _state = State::START;
    int _score = 0;
    int _timer = 0;
    float _tick = 0.0f; // time since the timer last went down
    std::vector<Contact> _contacts;

    void setScore(int score);
    void setTimer(int timer);
    void setState(State state);

    void tickTimer();
    void moveContact(Contact& contact, float dt) const;
    void spawn(Contact& contact);
    void respawnAwayFrom(Contact& contact, const ax::Vec2& position);
    ax::Vec2 randomLocation();
    ax::Vec2 randomDirection();
};
