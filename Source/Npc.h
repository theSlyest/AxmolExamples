#pragma once

#include "axmol.h"
#include "Character.h"

class Npc : public Character
{
public:
    bool init() override;
    
    CREATE_FUNC(Npc);

protected:
    ax::Color4F getCharacterColor() const override;
};
