#include "Player.h"

USING_NS_AX;

bool Player::init() 
{
    if (!Character::init())
        return false;

    return true;
}

ax::Color4F Player::getCharacterColor() const { return Color4F::BLUE; }
