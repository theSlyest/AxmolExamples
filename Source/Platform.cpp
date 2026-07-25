#include "Platform.h"

#include "Constants.h"

USING_NS_AX;

Platform* Platform::create(const float width, const float height)
{
    if (auto* pRet = new (std::nothrow) Platform(); pRet && pRet->init(width, height))
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

bool Platform::init(const float width, const float height)
{
    if (!Sprite::initWithFile("box.png"))
        return false;

    setScale(width / _contentSize.width, height / _contentSize.height);
    setColor(Color3B(0, 255, 0));

    const auto physicsBody = PhysicsBody::createBox(_contentSize);
    physicsBody->getShape(0)->setFriction(0.0f);
    physicsBody->getShape(0)->setRestitution(0.0f);
    physicsBody->setDynamic(false);
    physicsBody->setCategoryBitmask(BITMASK_PLATFORM);
    physicsBody->setCollisionBitmask(BITMASK_PLAYER); // Enable collision for all categories
    physicsBody->setContactTestBitmask(BITMASK_PLAYER);
    setPhysicsBody(physicsBody);

    return true;
}
