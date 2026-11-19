#include "BomberEnemy.h"

#include "ActorModel.h"
#include "GridModel.h"

USING_NS_AX;

const Color4F BomberEnemy::COLOR(0.33f, 0.79f, 0.21f, 1.0f);

bool BomberEnemy::init()
{
    if (!DrawNode::init())
        return false;

    setContentSize(Size(GridModel::TILE_SIZE, GridModel::TILE_SIZE));

    constexpr float radius = ActorModel::SIZE / 2.0f;
    drawSolidCircle(Vec2(GridModel::TILE_SIZE / 2.0f, GridModel::TILE_SIZE / 2.0f), radius, 0.0f, radius * M_PI, COLOR);

    return true;
}
