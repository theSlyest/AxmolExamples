#include "Paddle.h"

using namespace ax;

const Size Paddle::PaddleSize(108.0f, 18.0f);
const Color4F Paddle::Color(0.4f, 0.7f, 1.0f, 1.0f); // Blue

bool Paddle::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !FilledRect::init(PaddleSize, Color) )
    {
        return false;
    }

    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    _minX = origin.x + PaddleSize.width / 2.0f;
    _maxX = origin.x + visibleSize.width - PaddleSize.width / 2.0f;

    return true;
}

void Paddle::update(float dt)
{
    if (_dx != 0) {
        if (_dx > 0) {
            setPositionX(std::min(_maxX, _position.x + _dx * Speed * dt));
        }
        else {
            setPositionX(std::max(_minX, _position.x + _dx * Speed * dt));
        }
    }
}