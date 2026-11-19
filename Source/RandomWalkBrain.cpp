#include "RandomWalkBrain.h"

#include <random>
#include <vector>

#include "Directions.h"

namespace {
size_t defaultPick(const size_t count)
{
    static std::mt19937 engine{ std::random_device{}() };
    return std::uniform_int_distribution<size_t>(0, count - 1)(engine);
}
}

RandomWalkBrain::RandomWalkBrain(Pick pick) : _pick(pick ? std::move(pick) : Pick(defaultPick))
{
}

ax::Vec2 RandomWalkBrain::chooseDirection(const GridModel& grid, const ax::Vec2& pos)
{
    std::vector<ax::Vec2> dirs;
    for (const auto& dir : DIRECTIONS) {
        if (grid.isSafe(GridModel::neighborPosition(pos, dir)))
            dirs.push_back(dir);
    }

    if (dirs.empty())
        return ax::Vec2::ZERO;

    return dirs[_pick(dirs.size())];
}
