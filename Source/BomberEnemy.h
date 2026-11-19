#pragma once

#include "2d/DrawNode.h"

// How an enemy looks: a green circle. The game moves it by setting its position.
class BomberEnemy : public ax::DrawNode
{
public:
    bool init() override;

    CREATE_FUNC(BomberEnemy);

private:
    static const ax::Color4F COLOR;
};
