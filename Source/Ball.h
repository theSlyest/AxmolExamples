#ifndef __BB_BALL_H__
#define __BB_BALL_H__

#include "axmol.h"

class Ball : public ax::DrawNode
{
public:
    static constexpr float Speed = 360.0f;
    static constexpr float Radius = 10.0f;

    virtual bool init();
    
    CREATE_FUNC(Ball);

    void resetVelocity();
    void reverseVelocityX() { _direction.x *= -1; }
    void reverseVelocityY() { _direction.y *= -1; }
    void setVelocityX(float vx);

    virtual void update(float dt) override;

private:
    static const ax::Color4F Color;

    ax::Vec2 _direction;
    float _minX, _maxX, _maxY;
};

#endif // __BB_BALL_H__
