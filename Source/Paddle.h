#ifndef __BB_PADDLE_H__
#define __BB_PADDLE_H__

#include "axmol.h"
#include "FilledRect.h"

class Paddle : public FilledRect
{
public:
    static constexpr float Speed = 480.0f;
    static const ax::Size PaddleSize;

    virtual bool init();
    
    CREATE_FUNC(Paddle);

    void goLeft() { if (_dx == 0) _dx = -1; }
    void goRight() { if (_dx == 0) _dx = 1; }
    void stop() { _dx = 0; }
    int getDirection() const { return _dx; }

    virtual void update(float dt) override;

private:
    static const ax::Color4F Color;

    int _dx;
    float _minX, _maxX;
};

#endif // __BB_PADDLE_H__
