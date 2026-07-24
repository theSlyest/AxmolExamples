#include "BomberBomb.h"

#include "GridTile.h"

USING_NS_AX;

const float BomberBomb::SIZE = 30.0f;
const Color4F BomberBomb::COLOR(0.28f, 0.36f, 0.45f, 1.0f);

BomberBomb* BomberBomb::create(BomberPlayer* player)
{
    BomberBomb *pRet = new(std::nothrow) BomberBomb();
    if (pRet && pRet->init(player))
    {
        pRet->autorelease();
        return pRet;
    }
    else
    {
        delete pRet;
        pRet = nullptr;
        return nullptr;
    }
}

void BomberBomb::releaseBomber()
{
    if (_player == nullptr)
        return;

    _player->clearBomb();
    _player->release();
    _player = nullptr;
}

// on "init" you need to initialize your instance
bool BomberBomb::init(BomberPlayer* player)
{
    if ( !DrawNode::init() )
    {
        return false;
    }

    setContentSize(Size(GridTile::SIZE, GridTile::SIZE));
    const float middle = GridTile::SIZE / 2.0f;
    const Vec2 center(middle, middle);
    const float radius = SIZE / 2.0f;
    drawSolidCircle(center, radius, 0.0f, radius * M_PI, COLOR);
    drawCircle(center, radius, 0.0f, radius * M_PI, false, Color4F::WHITE);

    _player = player;
    if (player != nullptr)
        player->retain();

    return true;
}