#pragma once

#include <functional>

#include "EnemyBrain.h"

// Picks a random free neighboring tile each time.
class RandomWalkBrain : public EnemyBrain
{
public:
    // Returns a number in [0, count). Uses a standard random engine when no source is given.
    using Pick = std::function<size_t(size_t count)>;

    explicit RandomWalkBrain(Pick pick = nullptr);

    ax::Vec2 chooseDirection(const GridModel& grid, const ax::Vec2& pos) override;

private:
    Pick _pick;
};
