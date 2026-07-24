#ifndef __BOMBERMAN_GRID_TILE_H__
#define __BOMBERMAN_GRID_TILE_H__

#include "axmol.h"
#include "BomberBomb.h"

enum class TileState { SAFE, WALL, STEEL, BOMB };

class GridTile : public ax::DrawNode
{
public:
    static constexpr float SIZE = 48.0f;

    virtual bool init(TileState state);
    
    static GridTile* create(TileState state);

    TileState getState() const { return _state; }
    void setState(TileState state);

    void setBomb(BomberBomb* bomb);
    void releaseBomb();

    void startFire();
    void stopFire();
    bool isOnFire() const { return _fire->isVisible(); }

    bool updateState(float dt);

private:
    static constexpr float FIRE_DELAY = 0.5f;
    static constexpr float BOMB_DELAY = 2.0f;
    static const std::map<TileState, ax::Color4F> COLORS;
    static const ax::Color4F BORDER_COLOR;
    static const ax::Color4F BOMB_COLOR;
    static const ax::Color4F FIRE_COLOR;

    TileState _state;
    bool _isOnFire;
    float _remaining;

    ax::DrawNode* _fire;
    BomberBomb* _bomb;

    void drawState(TileState state);
};

#endif // __BOMBERMAN_GRID_TILE_H__
