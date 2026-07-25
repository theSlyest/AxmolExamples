#include "Character.h"

USING_NS_AX;

const Size Character::SIZE(16.0f, 16.0f);

bool Character::init() 
{
    if (!DrawNode::init())
        return false;

    setContentSize(SIZE);
    drawSolidRect(Vec2::ZERO, Vec2(SIZE), getCharacterColor());
    _direction = Vec2::ZERO;

    return true;
}

Vec2 Character::nextPosition(const float dt) const
{ return getPosition() + _direction * dt * Character::SPEED; }
