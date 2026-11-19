#pragma once

#include <functional>
#include <utility>
#include <vector>

#include "math/Vec2.h"

enum class TileState { SAFE, WALL, STEEL, BOMB };

using GridCoords = std::pair<int, int>;

// The map and everything that happens on it: walls, steel, bombs ticking down,
// explosions spreading and chain-reacting, fire burning out.
//
// This class has no nodes and no drawing, so it can be read and tested on its
// own. A view (BomberGrid) listens to onTileChanged and draws the tiles.
class GridModel
{
public:
    static constexpr int COLS = 13;
    static constexpr int ROWS = 11;
    static constexpr int BLAST_RANGE = 2; // tiles reached by an explosion in each direction
    static constexpr float TILE_SIZE = 48.0f;
    static constexpr float BOMB_DELAY = 2.0f; // seconds before a bomb explodes
    static constexpr float FIRE_DELAY = 0.5f; // seconds a tile keeps burning
    static constexpr int NO_OWNER = -1;

    // Tile coordinates <-> positions in the grid's local space.
    static ax::Vec2 coordsToPosition(const GridCoords& coords);
    static GridCoords positionToCoords(const ax::Vec2& position);

    // Position of the tile next to `position` in the given unit direction.
    static ax::Vec2 neighborPosition(const ax::Vec2& position, const ax::Vec2& direction);

    // Tile under the centre of a tile-sized object whose bottom-left corner is at `position`.
    static GridCoords centerCoords(const ax::Vec2& position);

    // Starts as a fresh map: steel around the edge and on every even/even tile,
    // walls everywhere else except for free corners to start in.
    GridModel();

    [[nodiscard]] TileState getState(int col, int row) const { return _tiles[col][row].state; }
    [[nodiscard]] TileState getState(const ax::Vec2& position) const;
    [[nodiscard]] bool isOnFire(int col, int row) const { return _tiles[col][row].fire; }
    [[nodiscard]] bool isOnFire(const ax::Vec2& position) const;
    // True for an empty tile that something can walk on (burning or not).
    [[nodiscard]] bool isSafe(const ax::Vec2& position) const { return getState(position) == TileState::SAFE; }
    // Who planted the bomb on this tile, or NO_OWNER.
    [[nodiscard]] int getBombOwner(int col, int row) const { return _tiles[col][row].owner; }

    // Restore walls and free corners, and put out every fire and bomb.
    void reset();

    void setState(int col, int row, TileState state);

    // Plant a bomb that explodes after BOMB_DELAY seconds. `owner` is reported
    // back through onBombReleased when the bomb is gone.
    void setBomb(int col, int row, int owner);

    // Blow up the tile and spread fire up to BLAST_RANGE tiles in each direction.
    // Fire stops at steel, destroys the first wall it meets, and sets off other bombs.
    void detonate(int col, int row);

    // Count down bombs and fires; bombs whose time is up explode.
    void update(float dt);

    // Notifications for the view.
    std::function<void(int col, int row)> onTileChanged;
    std::function<void(int owner)> onBombReleased;

private:
    struct Tile
    {
        TileState state = TileState::SAFE;
        bool fire = false;
        float remaining = 0.0f; // time left on a bomb or a fire
        int owner = NO_OWNER;
    };

    std::vector<std::vector<Tile>> _tiles;

    static bool isFixed(int col, int row);
    static TileState initialState(int col, int row);

    void releaseBomb(int col, int row);
    void startFire(int col, int row);
    void stopFire(int col, int row);
    bool tryFire(int col, int row);
    bool updateTile(int col, int row, float dt);
    void notify(int col, int row) const;
};
