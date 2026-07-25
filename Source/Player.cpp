#include "Player.h"

#include "Constants.h"

USING_NS_AX;

Player* Player::create(bool isClone)
{
    Player *pRet = new(std::nothrow) Player();
    if (pRet && pRet->init(isClone))
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

bool Player::init(bool isClone)
{
    if (!Sprite::initWithFile("box.png"))
    {
        return false;
    }

    setScale(0.5f);
    if (isClone) {
        setColor(Color3B(0, 255, 255)); // Cyan
    }
    else {
        setColor(Color3B(255, 255, 255)); // White
    }
    auto physicsBody = PhysicsBody::createBox(getContentSize());
    physicsBody->getShape(0)->setFriction(0.0f);
    physicsBody->getShape(0)->setRestitution(0.0f);
    physicsBody->setDynamic(true);
    physicsBody->setCategoryBitmask(BITMASK_PLAYER);
    physicsBody->setCollisionBitmask(BITMASK_PLATFORM | BITMASK_GOAL);
    physicsBody->setContactTestBitmask(BITMASK_PLATFORM | BITMASK_GOAL);
    physicsBody->setLinearDamping(0.0f);
    physicsBody->setMass(1.0f);
    physicsBody->setRotationEnable(false);
    setPhysicsBody(physicsBody);

    _isGrounded = true;

    return true;
}

void Player::startMovingLeft()
{
    if (_xMove == XMove::None) {
        _xMove = XMove::Left;
        _physicsBody->setVelocity(Vec2(-X_VELOCITY, 0.0f));
    }
}

void Player::startMovingRight()
{
    if (_xMove == XMove::None) {
        _xMove = XMove::Right;
        _physicsBody->setVelocity(Vec2(X_VELOCITY, 0.0f));
    }
}

void Player::stopMovingLeft() 
{
    if (_xMove == XMove::Left) {
        _xMove = XMove::None;
        _physicsBody->setVelocity(Vec2::ZERO);
    }
}

void Player::stopMovingRight() 
{
    if (_xMove == XMove::Right) {
        _xMove = XMove::None;
        _physicsBody->setVelocity(Vec2::ZERO);
    }
}

void Player::jump()
{
    if (!_isGrounded)
        return;

    _physicsBody->applyImpulse(Vec2(0, 250.0f)); // Apply an upward impulse to jump
    _isGrounded = false;
}

void Player::fall()
{
    _isGrounded = false;
}

void Player::land()
{
    _isGrounded = true;
}
