#pragma once

#include "math/Vec2.h"

// The movement state shared by the player and the enemies: where an actor is,
// which way it is heading, and which tile it is walking to.
//
// This class has no nodes and no drawing, so it can be read and tested on its
// own. Views copy its position every frame.
class ActorModel
{
public:
    static constexpr float SIZE = 36.0f;       // the actor's body, centred in a tile
    static constexpr float OFFSET = 6.0f;      // gap between the body and the tile's edge
    static constexpr float SPAWN_DELAY = 0.55f; // seconds of invincibility after spawning

    explicit ActorModel(float speed = 0.0f) : _speed(speed) {}

    void setPosition(const ax::Vec2& position) { _position = position; }
    [[nodiscard]] ax::Vec2 getPosition() const { return _position; }

    void setDirection(const ax::Vec2& direction) { _direction = direction; }
    [[nodiscard]] ax::Vec2 getDirection() const { return _direction; }

    void setDestination(const ax::Vec2& destination) { _destination = destination; }
    [[nodiscard]] ax::Vec2 getDestination() const { return _destination; }

    // True once `nextPos` is at or past the destination along the direction of travel.
    [[nodiscard]] bool hasReachedDestination(const ax::Vec2& nextPos) const;

    [[nodiscard]] float getSpeed() const { return _speed; }

    void setInvincible(bool invincible) { _remaining = invincible ? SPAWN_DELAY : 0.0f; }
    [[nodiscard]] bool isInvincible() const { return _remaining > 0.0f; }

    void update(float dt);

private:
    float _speed;
    ax::Vec2 _position;
    ax::Vec2 _direction;
    ax::Vec2 _destination;
    float _remaining = 0.0f;
};
