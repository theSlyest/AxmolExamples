#ifndef __BOMBERMAN_PLAYER_H__
#define __BOMBERMAN_PLAYER_H__

#include "axmol.h"

class BomberPlayer : public ax::DrawNode
{
public:
    static constexpr float SIZE = 36.0f;
    static constexpr float PLAYER_SPEED = 200.0f;
    static constexpr float ENEMY_SPEED = 100.0f;
    static constexpr float OFFSET = 6.0f;
    static constexpr float SPAWN_DELAY = 0.55f;

    virtual bool init(bool isEnemy);
    
    // implement the "static create()" method manually
    static BomberPlayer* create(bool isEnemy = false);

    void setDirection(const ax::Vec2& direction) { _direction = direction; }
    ax::Vec2 getDirection() const { return _direction; }
    bool isIdle() const { return _direction == ax::Vec2::ZERO; }

    void setDestination(const ax::Vec2& dest) { _destination = dest; }
    ax::Vec2 getDestination() const { return _destination; }

    bool hasReachedDestination(const ax::Vec2 &nextPos);

    void dropBomb();
    void clearBomb();

    bool hasBomb() const { return _hasBomb; }

    void update(float dt) override;

    void setInvincible(bool invincible) { _remaining = invincible ? SPAWN_DELAY : 0.0f; }
    bool isInvincible() const { return _remaining > 0.0f; }

    float getSpeed() const { return _speed; }

private:
    static const ax::Color4F PLAYER_COLOR;
    static const ax::Color4F ENEMY_COLOR;
    ax::Vec2 _direction;
    ax::Vec2 _destination;

    float _speed;
    float _remaining;
    bool _hasBomb;
};

#endif // __BOMBERMAN_PLAYER_H__
