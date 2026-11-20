#pragma once

#include "2d/DrawNode.h"

class Contact : public ax::DrawNode
{
public:
    static constexpr float RADIUS = 7.0f;
    static constexpr float SPEED = 16.0f;  // pixels per second

    bool init() override;

    CREATE_FUNC(Contact);

    void setDirection(ax::Vec2 direction) { _direction = direction; }
    ax::Vec2 getDirection() const { return _direction; }

    void setRandomDirection();

private:
    ax::Vec2 _direction;
};
