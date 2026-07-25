#pragma once

#include "axmol.h"

class Character : public ax::DrawNode
{
public:
    static constexpr float SPEED = 160.0f;

    static const ax::Size SIZE;

    bool init() override;

    ax::Vec2 nextPosition(float dt) const;

    void setDirection(ax::Vec2 direction) { _direction = direction; }
    ax::Vec2 getDirection() const { return _direction; }

protected:
    ax::Vec2 _direction;

    virtual ax::Color4F getCharacterColor() const = 0;
};
