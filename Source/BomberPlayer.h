#pragma once

#include "2d/DrawNode.h"

// How the player looks: a blue square. The game moves it by setting its position.
class BomberPlayer : public ax::DrawNode
{
public:
    bool init() override;

    CREATE_FUNC(BomberPlayer);

private:
    static const ax::Color4F COLOR;
};
