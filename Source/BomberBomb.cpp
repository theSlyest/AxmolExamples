#include "BomberBomb.h"

#include "GridModel.h"

USING_NS_AX;

const Color4F BomberBomb::COLOR(0.28f, 0.36f, 0.45f, 1.0f);

bool BomberBomb::init()
{
    if (!DrawNode::init())
        return false;

    setContentSize(Size(GridModel::TILE_SIZE, GridModel::TILE_SIZE));
    constexpr float middle = GridModel::TILE_SIZE / 2.0f;
    constexpr Vec2 center(middle, middle);
    constexpr float radius = SIZE / 2.0f;
    drawSolidCircle(center, radius, 0.0f, radius * M_PI, COLOR);
    drawCircle(center, radius, 0.0f, radius * M_PI, false, Color4F::WHITE);

    return true;
}
