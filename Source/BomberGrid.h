#ifndef __BOMBERMAN_GRID_H__
#define __BOMBERMAN_GRID_H__

#include "axmol.h"
#include "GridTile.h"

using GridCoords = std::pair<int, int>;

class BomberGrid : public ax::Node
{
public:
    static constexpr int COLS = 13;
    static constexpr int ROWS = 11;

    virtual bool init();
    
    // implement the "static create()" method manually
    CREATE_FUNC(BomberGrid);

    ax::Vec2 getTilePosition(const int col, const int row) const { return _tiles[col][row]->getPosition(); }
    TileState getTileState(const int col, const int row) const  { return _tiles[col][row]->getState(); }
    void setTileState(const int col, const int row, const TileState state) { _tiles[col][row]->setState(state); }

    GridTile* getTile(const ax::Vec2& position);
    GridCoords getCoords(const ax::Vec2& position);

    void reset();

    void detonate(const int col, const int row);
    void detonate(GridTile* tile);

    void update(float dt) override;

private:

    const std::set<GridCoords> SAFE_COORDS {
        {1, 1}, {1, 2}, {2, 1},
        {1, 8}, {1, 9}, {2, 9},
        {10, 1}, {11, 1}, {11, 2},
        {10, 9}, {11, 8}, {11, 9}
    };

    std::vector<std::vector<GridTile*>> _tiles;

    bool tryFire(const int col, const int row);
};

#endif // __BOMBERMAN_GRID_H__
