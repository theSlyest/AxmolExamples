#pragma once

#include "2d/DrawNode.h"

class Emitter : public ax::DrawNode
{
public:
    static constexpr float LIGHT_RADIUS = 6.0f;
    static const ax::Vec2 SIZE;
    static const ax::Color4F COLOR;
    static const ax::Color4F LIGHT_COLOR;

    bool init() override;
    
    // implement the "static create()" method manually
    CREATE_FUNC(Emitter);
};
