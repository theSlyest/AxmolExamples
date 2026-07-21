
#include "Ball.h"

using namespace ax;

const Color4F Ball::Color(0.9f, 0.9f, 0.9f, 1.0f); // Light gray

bool Ball::init()
{
    if ( !DrawNode::init() )
    {
        return false;
    }

    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    _minX = origin.x + Radius;
    _maxX = origin.x + visibleSize.width - Radius;
    _maxY = origin.y + visibleSize.height - Radius;

    float diameter = 2.0f * Radius;
    setContentSize(Size(diameter, diameter));
    drawSolidCircle(Vec2(Radius, Radius), Radius, 0.0f, 32, Color);

    resetVelocity();

    return true;
}

void Ball::resetVelocity()
{
    float radAngle = 1.3f;
    _direction.x = cos(radAngle) * Speed;
    _direction.y = sin(radAngle) * Speed;
}

void Ball::setVelocityX(float vx)
{
    const float ux = vx / Speed;
    const float uy = sqrtf(1.0f - (ux * ux));
    _direction.x = vx;
    _direction.y = uy * Speed;
}

void Ball::update(float dt)
{
    Vec2 pos = _position + dt * _direction;
    setPosition(pos);

    if (pos.x <= _minX || pos.x >= _maxX) {
        _direction.x *= -1;
    }

    if (pos.y >= _maxY) {
        _direction.y *= -1;
    }
}