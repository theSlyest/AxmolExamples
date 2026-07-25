#include "Goal.h"

#include "Constants.h"

USING_NS_AX;

Goal* Goal::create(const bool isCheckbox)
{
    if (auto pRet = new (std::nothrow) Goal(); pRet && pRet->init(isCheckbox))
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

bool Goal::init(const bool isCheckbox)
{
    if (!Sprite::initWithFile("box.png"))
    {
        return false;
    }

    setScale(0.5f);
    if (!isCheckbox)
        setColor(Color3B(255, 255, 0));

    const auto physicsBody = PhysicsBody::createEdgeBox(_contentSize);
    physicsBody->setCategoryBitmask(BITMASK_GOAL);
    physicsBody->setContactTestBitmask(BITMASK_PLAYER);
    setPhysicsBody(physicsBody);

    return true;
}

