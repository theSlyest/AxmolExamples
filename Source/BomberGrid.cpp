#include "BomberGrid.h"

USING_NS_AX;

// on "init" you need to initialize your instance
bool BomberGrid::init()
{
    if ( !Node::init() )
    {
        return false;
    }

    setContentSize(Size(COLS * GridTile::SIZE, ROWS * GridTile::SIZE));

    for (int c = 0; c < COLS; ++c) {
        std::vector<GridTile*> col;
        for (int r = 0; r < ROWS; ++r) {
            TileState state;
            if (c == 0 || c == COLS - 1 || r == 0 || r == ROWS - 1)
                state = TileState::STEEL;
            else if (c % 2 == 0 && r % 2 == 0)
                state = TileState::STEEL;
            else if (SAFE_COORDS.count(GridCoords(c, r)))
                state = TileState::SAFE;
            else
                state = TileState::WALL;

            auto tile = GridTile::create(state);
            tile->setPosition(c * GridTile::SIZE, r * GridTile::SIZE);
            addChild(tile);
            col.push_back(tile);
        }
        _tiles.push_back(col);
    }

    return true;
}

GridTile* BomberGrid::getTile(const ax::Vec2 &position)
{
    return _tiles[position.x / GridTile::SIZE][position.y / GridTile::SIZE];
}

GridCoords BomberGrid::getCoords(const ax::Vec2 &position)
{
    return { position.x / GridTile::SIZE, position.y / GridTile::SIZE };
}

void BomberGrid::reset()
{
    for (int c = 0; c < COLS; ++c) {
        for (int r = 0; r < ROWS; ++r) {
            if (c == 0 || c == COLS - 1 || r == 0 || r == ROWS - 1)
                continue;

            if (c % 2 == 0 && r % 2 == 0)
                continue;

            auto tile = _tiles[c][r];
            tile->stopFire();
            tile->releaseBomb();

            if (SAFE_COORDS.count(GridCoords(c, r)))
                continue;

            tile->setState(TileState::WALL);
        }
    }
}

bool BomberGrid::tryFire(const int col, const int row)
{
    auto tile = _tiles[col][row];
    TileState state = tile->getState();
    if (state == TileState::BOMB)
        detonate(col, row);
    else if (state != TileState::STEEL) {
        tile->startFire();
        if (state == TileState::SAFE)
            return true;
    }
    
    return false;
}

void BomberGrid::detonate(const int col, const int row)
{
    _tiles[col][row]->startFire();

    bool goLeft = true;
    bool goUp = true;
    bool goRight = true;
    bool goDown = true;

    for (int i = 1; i < 3; ++i) {
        if (goRight) {
            const int next = col + i;
            goRight = tryFire(next, row);
        }

        if (goLeft) {
            const int next = col - i;
            goLeft = tryFire(next, row);
        }

        if (goUp) {
            const int next = row + i;
            goUp = tryFire(col, next);
        }

        if (goDown) {
            const int next = row - i;
            goDown = tryFire(col, next);
        }
    }
}

void BomberGrid::detonate(GridTile *tile)
{
    const GridCoords& coords = getCoords(tile->getPosition());
    detonate(coords.first, coords.second);
}

void BomberGrid::update(float dt)
{
    for (int c = 1; c < COLS - 1; ++c) {
        for (int r = 1; r < ROWS - 1; ++r) {
            auto tile = _tiles[c][r];
            if (tile->getState() == TileState::BOMB || tile->isOnFire())
                if (tile->updateState(dt))
                    detonate(c, r);
        }
    }
}
