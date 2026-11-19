#include "GridModel.h"

#include <set>

USING_NS_AX;

namespace {
// Free tiles in each corner so that nobody starts boxed in or burning.
const std::set<GridCoords> SAFE_COORDS {
    {1, 1}, {1, 2}, {2, 1},
    {1, 8}, {1, 9}, {2, 9},
    {10, 1}, {11, 1}, {11, 2},
    {10, 9}, {11, 8}, {11, 9}
};
}

Vec2 GridModel::coordsToPosition(const GridCoords& coords)
{
    return { static_cast<float>(coords.first) * TILE_SIZE, static_cast<float>(coords.second) * TILE_SIZE };
}

GridCoords GridModel::positionToCoords(const Vec2& position)
{
    return { static_cast<int>(position.x / TILE_SIZE), static_cast<int>(position.y / TILE_SIZE) };
}

Vec2 GridModel::neighborPosition(const Vec2& position, const Vec2& direction)
{
    return position + direction * TILE_SIZE;
}

GridCoords GridModel::centerCoords(const Vec2& position)
{
    return positionToCoords(position + Vec2(TILE_SIZE, TILE_SIZE) / 2.0f);
}

bool GridModel::isFixed(const int col, const int row)
{
    return col == 0 || col == COLS - 1 || row == 0 || row == ROWS - 1 || (col % 2 == 0 && row % 2 == 0);
}

TileState GridModel::initialState(const int col, const int row)
{
    if (isFixed(col, row))
        return TileState::STEEL;

    return SAFE_COORDS.contains(GridCoords(col, row)) ? TileState::SAFE : TileState::WALL;
}

GridModel::GridModel() : _tiles(COLS, std::vector<Tile>(ROWS))
{
    for (int col = 0; col < COLS; ++col)
        for (int row = 0; row < ROWS; ++row)
            _tiles[col][row].state = initialState(col, row);
}

TileState GridModel::getState(const Vec2& position) const
{
    const auto [col, row] = positionToCoords(position);
    return getState(col, row);
}

bool GridModel::isOnFire(const Vec2& position) const
{
    const auto [col, row] = positionToCoords(position);
    return isOnFire(col, row);
}

void GridModel::reset()
{
    for (int col = 0; col < COLS; ++col) {
        for (int row = 0; row < ROWS; ++row) {
            if (isFixed(col, row))
                continue;

            stopFire(col, row);
            releaseBomb(col, row);
            setState(col, row, initialState(col, row));
        }
    }
}

void GridModel::setState(const int col, const int row, const TileState state)
{
    _tiles[col][row].state = state;
    notify(col, row);
}

void GridModel::setBomb(const int col, const int row, const int owner)
{
    Tile& tile = _tiles[col][row];
    tile.state = TileState::BOMB;
    tile.owner = owner;
    tile.remaining = BOMB_DELAY;
    notify(col, row);
}

void GridModel::releaseBomb(const int col, const int row)
{
    Tile& tile = _tiles[col][row];
    if (tile.owner == NO_OWNER)
        return;

    const int owner = tile.owner;
    tile.owner = NO_OWNER;
    if (onBombReleased)
        onBombReleased(owner);
}

void GridModel::startFire(const int col, const int row)
{
    if (_tiles[col][row].state == TileState::BOMB)
        releaseBomb(col, row);

    Tile& tile = _tiles[col][row];
    tile.state = TileState::SAFE;
    tile.remaining = FIRE_DELAY;
    tile.fire = true;
    notify(col, row);
}

void GridModel::stopFire(const int col, const int row)
{
    _tiles[col][row].fire = false;
    notify(col, row);
}

bool GridModel::tryFire(const int col, const int row)
{
    const TileState state = _tiles[col][row].state;
    if (state == TileState::BOMB)
        detonate(col, row);
    else if (state != TileState::STEEL) {
        startFire(col, row);
        if (state == TileState::SAFE)
            return true;
    }

    return false;
}

void GridModel::detonate(const int col, const int row)
{
    startFire(col, row);

    bool goLeft = true;
    bool goUp = true;
    bool goRight = true;
    bool goDown = true;

    for (int i = 1; i <= BLAST_RANGE; ++i) {
        if (goRight)
            goRight = tryFire(col + i, row);

        if (goLeft)
            goLeft = tryFire(col - i, row);

        if (goUp)
            goUp = tryFire(col, row + i);

        if (goDown)
            goDown = tryFire(col, row - i);
    }
}

bool GridModel::updateTile(const int col, const int row, const float dt)
{
    Tile& tile = _tiles[col][row];
    if (tile.state == TileState::BOMB || tile.fire) {
        tile.remaining -= dt;
        if (tile.remaining <= 0.0f) {
            if (tile.state == TileState::BOMB)
                return true;

            stopFire(col, row);
        }
    }

    return false;
}

void GridModel::update(const float dt)
{
    for (int col = 1; col < COLS - 1; ++col) {
        for (int row = 1; row < ROWS - 1; ++row) {
            const Tile& tile = _tiles[col][row];
            if ((tile.state == TileState::BOMB || tile.fire) && updateTile(col, row, dt))
                detonate(col, row);
        }
    }
}

void GridModel::notify(const int col, const int row) const
{
    if (onTileChanged)
        onTileChanged(col, row);
}
