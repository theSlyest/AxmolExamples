#pragma once

#include "2d/DrawNode.h"
#include "GridModel.h"

// How one tile of the map looks: a colored square with a border, and a red
// overlay while it burns. It only draws what it is told to show.
class GridTile : public ax::DrawNode
{
public:
    static constexpr float SIZE = GridModel::TILE_SIZE;

    bool init() override;

    CREATE_FUNC(GridTile);

    void show(TileState state, bool onFire);

private:
    static const std::map<TileState, ax::Color4F> COLORS;
    static const ax::Color4F BORDER_COLOR;
    static const ax::Color4F FIRE_COLOR;

    TileState _state = TileState::SAFE;
    bool _shown = false;
    ax::DrawNode* _fire = nullptr;
};
