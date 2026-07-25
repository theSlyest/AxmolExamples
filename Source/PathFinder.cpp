#include "PathFinder.h"

#include <algorithm>
#include <climits>
#include <queue>

using namespace std;

// Manhattan distance heuristic
int PathFinder::heuristic(const Coords& a, const Coords& b)
{
    return abs(a.x - b.x) + abs(a.y - b.y);
}

vector<Coords> PathFinder::run(const std::vector<std::vector<bool>>& grid, const Coords& start, const Coords& target) 
{
    const int rows = grid.size();
    const int cols = grid[0].size();

    // Orthogonal directions: Up, Down, Left, Right

    // Priority queue to explore the current with the lowest 'f' score first
    priority_queue<Node, vector<Node>, greater<>> openSet;

    // Keep track of visited cells
    vector<vector<bool>> closedSet(rows, vector<bool>(cols, false));

    // Store nodes to reconstruct the path later
    vector<vector<Coords>> cameFrom(rows, vector<Coords>(cols, {-1, -1}));

    // Keep track of g_scores, initialized to a very high value
    vector<vector<int>> gScore(rows, vector<int>(cols, INT_MAX));
    gScore[start.x][start.y] = 0;

    openSet.push({ .pos = start, .g = 0, .h = heuristic(start, target) });

    while (!openSet.empty()) {
        Node current = openSet.top();
        openSet.pop();

        // If we reached the target
        if (current.pos == target) {
            return reconstructPath(cameFrom, target);
        }

        // Skip if already visited
        if (closedSet[current.pos.x][current.pos.y])
            continue;

        // Mark as visited
        closedSet[current.pos.x][current.pos.y] = true;

        for (int i = 0; i < 4; ++i) {
            int dy[] = {0, 0, -1, 1};
            int dx[] = {-1, 1, 0, 0};
            int nx = current.pos.x + dx[i];
            int ny = current.pos.y + dy[i];

            // Check boundaries, walls, and whether it's already in the closedSet
            if (nx >= 0 && nx < rows && ny >= 0 && ny < cols && 
                !grid[nx][ny] && !closedSet[nx][ny]) {

                int tentative_g = current.g + 1; // Orthogonal moves cost 1

                // If this is a better path to the neighbor
                if (tentative_g < gScore[nx][ny]) {
                    cameFrom[nx][ny] = current.pos;
                    gScore[nx][ny] = tentative_g;
        
                    Coords neighbor{nx, ny};
                    int hScore = heuristic(neighbor, target);
                    openSet.push({neighbor, tentative_g, hScore});
                }
            }
        }
    }

    return {};
}

std::vector<Coords> PathFinder::reconstructPath(const std::vector<std::vector<Coords>>& cameFrom, const Coords& target)
{
    vector<Coords> path;
    Coords current = target;
    bool isTarget = true;
    // Trace backwards until we hit an unassigned parent cell (-1, -1)
    while (cameFrom[current.x][current.y].x != -1) {
        if (isTarget)
            isTarget = false;
        else
            path.push_back(current);
        current = cameFrom[current.x][current.y];
    }
    std::reverse(path.begin(), path.end());
    return path;
}