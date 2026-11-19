#pragma once

#include "2d/DrawNode.h"
#include "BomberBomb.h"
#include "GridModel.h"


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
    void stopFire() const;
    bool isOnFire() const { return _fire->isVisible(); }

    bool updateState(float dt);

private:
    static constexpr float FIRE_DELAY = 0.5f;
    static constexpr float BOMB_DELAY = 2.0f;
    static const std::map<TileState, ax::Color4F> COLORS;
    static const ax::Color4F BORDER_COLOR;
    static const ax::Color4F FIRE_COLOR;

    TileState _state = TileState::SAFE;
    float _remaining = 0;

    ax::DrawNode* _fire = nullptr;
    BomberBomb* _bomb   = nullptr;

    void drawState(TileState state);
};
