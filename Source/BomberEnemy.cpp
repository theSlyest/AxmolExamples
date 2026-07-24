#include "BomberEnemy.h"

#include "GridTile.h"

USING_NS_AX;

const float BomberEnemy::SIZE = 36.0f;
const float BomberEnemy::SPEED = 100.0f;

const Color4F BomberEnemy::COLOR(0.33f, 0.79f, 0.21f, 1.0f);

// on "init" you need to initialize your instance
bool BomberEnemy::init()
{
    if ( !DrawNode::init() )
    {
        return false;
    }

    setContentSize(Size(GridTile::SIZE, GridTile::SIZE));
    const float radius = SIZE / 2.0f;
    drawSolidCircle(Vec2(GridTile::SIZE / 2.0f, GridTile::SIZE / 2.0f), radius, 0.0f, radius * M_PI, COLOR);

    return true;
}