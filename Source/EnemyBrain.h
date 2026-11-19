#pragma once

#include "GridModel.h"

// Decides where an enemy goes next. Implement this to give enemies a new
// behavior (chase the player, flee from bombs, ...) without touching the
// movement code in BomberGame.
class EnemyBrain
{
public:
    virtual ~EnemyBrain() = default;

    // Called when an enemy stands on (or is about to reach) the tile at `pos`.
    // Returns a unit grid direction, or Vec2::ZERO to stay still.
    virtual ax::Vec2 chooseDirection(const GridModel& grid, const ax::Vec2& pos) = 0;
};
