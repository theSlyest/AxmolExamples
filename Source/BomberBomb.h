#pragma once

#include "2d/DrawNode.h"

// A bomb sitting on a tile: a dark disc with a white outline.
class BomberBomb : public ax::DrawNode
{
public:
    bool init() override;

    CREATE_FUNC(BomberBomb);

private:
    static constexpr float SIZE = 30.0f;
    static const ax::Color4F COLOR;
};
