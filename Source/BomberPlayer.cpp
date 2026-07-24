#include "BomberPlayer.h"

#include "GridTile.h"

USING_NS_AX;

const Color4F BomberPlayer::PLAYER_COLOR = Color4F(0.28f, 0.36f, 0.94f, 1.0f);
const Color4F BomberPlayer::ENEMY_COLOR = Color4F(0.33f, 0.79f, 0.21f, 1.0f);

BomberPlayer* BomberPlayer::create(bool isEnemy)
{
    BomberPlayer *pRet = new(std::nothrow) BomberPlayer();
    if (pRet && pRet->init(isEnemy))
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

// on "init" you need to initialize your instance
bool BomberPlayer::init(bool isEnemy)
{
    if ( !DrawNode::init() )
    {
        return false;
    }

    setContentSize(Size(GridTile::SIZE, GridTile::SIZE));

    if (isEnemy) {
        const float radius = SIZE / 2.0f;
        drawSolidCircle(Vec2(GridTile::SIZE / 2.0f, GridTile::SIZE / 2.0f), radius, 0.0f, radius * M_PI, ENEMY_COLOR);
        _speed = ENEMY_SPEED;
    } 
    else {
        float offset = (GridTile::SIZE - SIZE) / 2.0f;
        drawSolidRect(Vec2(offset, offset), Vec2(GridTile::SIZE - offset, GridTile::SIZE - offset), PLAYER_COLOR);
        _speed = PLAYER_SPEED;
    }

    _destination = Vec2::ZERO;
    _direction = Vec2::ZERO;
    _hasBomb = false;
    _remaining = 0.0f;

    return true;
}


bool BomberPlayer::hasReachedDestination(const Vec2 &nextPos)
{
    if (_direction.x > 0.0f && nextPos.x >= _destination.x)
        return true;

    if (_direction.x < 0.0f && nextPos.x <= _destination.x)
        return true;

    if (_direction.y > 0.0f && nextPos.y >= _destination.y)
        return true;

    if (_direction.y < 0.0f && nextPos.y <= _destination.y)
        return true;

    return false;
}

void BomberPlayer::dropBomb()
{
    _hasBomb = true;
}

void BomberPlayer::clearBomb()
{
    _hasBomb = false;
}

void BomberPlayer::update(float dt)
{
    if (_remaining > 0.0f)
        _remaining -= dt;
}
