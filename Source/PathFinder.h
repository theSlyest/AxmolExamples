#pragma once

#include <vector>

struct Coords {
    int x, y;

    bool operator ==(const Coords& other) const {
        return x == other.x && y == other.y;
    }
};

struct Node {
    Coords pos; // Coords
    int g; // Cost from the start
    int h; // Heuristic: estimated cost
    [[nodiscard]] int f() const { return g + h; } // Total cost

    bool operator >(const Node& other) const {
        return f() > other.f();
    }
};

class PathFinder
{
public:
    static std::vector<Coords> run(const std::vector<std::vector<bool>>& grid, const Coords& start, const Coords& target);

private:

    static int heuristic(const Coords& a, const Coords& b);
    static std::vector<Coords> reconstructPath(const std::vector<std::vector<Coords>>& cameFrom, const Coords& target);
};