#pragma once

#include "axmol.h"
#include "Character.h"

class Player : public Character
{
public:
    bool init() override;
    
    CREATE_FUNC(Player);

    bool isMoving() const { return _direction != ax::Vec2::ZERO; }

protected:
    ax::Color4F getCharacterColor() const override;
};
