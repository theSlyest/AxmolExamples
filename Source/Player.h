#pragma once

#include "axmol.h"

class Player : public ax::Sprite
{
public:
    enum class XMove { None, Left, Right };

    virtual bool init(bool isClone);

    // implement the "static create()" method manually
    static Player* create(bool isClone = false);

    void startMovingLeft();
    void startMovingRight();
    void stopMovingLeft();
    void stopMovingRight();
    void jump();
    void fall();
    void land();

private:
    const float X_VELOCITY = 48.0f;

    XMove _xMove = XMove::None;
    bool _isGrounded = false;
};
