#include "BomberPlayer.h"

#include "ActorModel.h"
#include "GridModel.h"

USING_NS_AX;

const Color4F BomberPlayer::COLOR(0.28f, 0.36f, 0.94f, 1.0f);

bool BomberPlayer::init()
{
    if (!DrawNode::init())
        return false;

    setContentSize(Size(GridModel::TILE_SIZE, GridModel::TILE_SIZE));

    constexpr float offset = (GridModel::TILE_SIZE - ActorModel::SIZE) / 2.0f;
    drawSolidRect(Vec2(offset, offset), Vec2(GridModel::TILE_SIZE - offset, GridModel::TILE_SIZE - offset), COLOR);

    return true;
}
