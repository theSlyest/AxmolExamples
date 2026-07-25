#include "Npc.h"

USING_NS_AX;

bool Npc::init() 
{
    if (!Character::init())
        return false;

    return true;
}

ax::Color4F Npc::getCharacterColor() const { return Color4F::GREEN; }
