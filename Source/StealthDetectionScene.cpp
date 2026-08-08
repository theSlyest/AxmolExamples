#include "StealthDetectionScene.h"

#include "Director.h"
#include "EventDispatcher.h"
#include "EventListenerKeyboard.h"
#include "2d/Layer.h"
#include "2d/Menu.h"
#include "2d/MenuItem.h"
#include "physics/PhysicsWorld.h"

USING_NS_AX;

// on "init" you need to initialize your instance
bool StealthDetectionScene::init()
{
    if (!Scene::initWithPhysics())
        return false;

    const auto physicsWorld = getPhysicsWorld();
    // physicsWorld->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);
    physicsWorld->setSubsteps(4);
    physicsWorld->setGravity(Vec2::ZERO);

    const Vec2& size = _director->getVisibleSize();
    const Vec2& origin = _director->getVisibleOrigin();

    const auto floor = LayerColor::create(Color4B(0, 128, 128, 255), size.x - 2.0f * FLOOR_POS.x, size.y - 2.0f * FLOOR_POS.y);
    floor->setPosition(FLOOR_POS);
    addChild(floor);

    _successLbl = Label::createWithTTF("SUCCESS", "fonts/arial.ttf", 36.0f);
    _successLbl->setPosition(origin.x + size.x / 2.0f, origin.y + size.y / 2.0f);
    _successLbl->setVisible(false);
    addChild(_successLbl, 1);

    initPlayer(origin);
    initGuard(origin);
    initDestination(origin);
    initWalls(origin);
    initResetButton(origin);

    const auto keyListener = EventListenerKeyboard::create();
    keyListener->onKeyPressed = [this](const EventKeyboard::KeyCode keyCode, Event*) { onKeyPressed(keyCode); };
    keyListener->onKeyReleased = [this](const EventKeyboard::KeyCode keyCode, Event*) { onKeyReleased(keyCode); };
    _eventDispatcher->addEventListenerWithSceneGraphPriority(keyListener, this);

    const auto onContactListener = EventListenerPhysicsContact::create();
    onContactListener->onContactBegin = [this](const PhysicsContact& contact) { return onContactBegin(contact); };
    onContactListener->onContactPreSolve = [this](const PhysicsContact& contact, PhysicsContactPreSolve&) { return onContactPresolve(contact); };
    _eventDispatcher->addEventListenerWithSceneGraphPriority(onContactListener, this);

    return true;
}

void StealthDetectionScene::update(float delta)
{
    _player->setPosition(_player->getPosition() + _playerVelocity * delta);
}

void StealthDetectionScene::initPlayer(const Vec2& origin)
{
    _player = DrawNode::create();
    _player->setContentSize(CHAR_SIZE);
    _player->drawSolidRect(Vec2::ZERO, Vec2(CHAR_SIZE.x, CHAR_SIZE.y), Color4F::BLUE);
    _player->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    _player->setPosition(origin + START_POS);
    addChild(_player);
    _playerPhysicsBody = PhysicsBody::createBox(CHAR_SIZE);
    _playerPhysicsBody->setDynamic(true);
    _playerPhysicsBody->setRotationEnable(false);
    _playerPhysicsBody->setLinearDamping(0.0f);
    _playerPhysicsBody->setAngularDamping(0.0f);
    _playerPhysicsBody->getShape(0)->setFriction(0.0f);
    _playerPhysicsBody->getShape(0)->setRestitution(0.0f);
    _playerPhysicsBody->setCategoryBitmask(PhysicsBitmask::PLAYER);
    _playerPhysicsBody->setCollisionBitmask(PhysicsBitmask::WALL);
    _playerPhysicsBody->setContactTestBitmask(PhysicsBitmask::GUARD_VISION | PhysicsBitmask::DESTINATION | PhysicsBitmask::WALL);
    _player->setPhysicsBody(_playerPhysicsBody);
}

void StealthDetectionScene::initGuard(const Vec2& origin)
{
    const auto guard = DrawNode::create();
    guard->setContentSize(CHAR_SIZE);
    guard->drawSolidRect(Vec2::ZERO, Vec2(CHAR_SIZE.x, CHAR_SIZE.y), Color4F::RED);
    guard->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    guard->setPosition(origin + GUARD_POS);
    addChild(guard);

    const float base = 2.0f * tanf(AX_DEGREES_TO_RADIANS(GUARD_VISION_ANGLE / 2.0f)) * GUARD_VISION_RANGE;
    const Vec2 points[3] {
        Vec2(0.0f, 0.0f),
        Vec2(0.0f, base),
        Vec2(GUARD_VISION_RANGE, base / 2.0f)
    };
    _guardVision = DrawNode::create();
    _guardVision->setContentSize(Vec2(GUARD_VISION_RANGE, base));
    _guardVision->drawSolidPoly(points, 3, Color4F(1.0f, 1.0f, 0.0f, 0.5f));
    _guardVision->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
    _guardVision->setPosition(GUARD_POS.x - CHAR_SIZE.x / 2.0f, GUARD_POS.y);
    addChild(_guardVision);

    const auto physicsBody = PhysicsBody::createPolygon(points, 3);
    physicsBody->setDynamic(false);
    physicsBody->setCategoryBitmask(PhysicsBitmask::GUARD_VISION);
    physicsBody->setContactTestBitmask(PhysicsBitmask::PLAYER);
    physicsBody->setPositionOffset(-Vec2(GUARD_VISION_RANGE / 2.0f, base / 2.0f));
    _guardVision->setPhysicsBody(physicsBody);
}

void StealthDetectionScene::initDestination(const Vec2 &origin)
{
    _destination = DrawNode::create();
    _destination->setContentSize(DEST_SIZE);
    _destination->drawRect(Vec2::ZERO, Vec2(DEST_SIZE), Color4F::GREEN);
    _destination->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    _destination->setPosition(origin + DEST_POS);
    addChild(_destination);

    const auto physicsBody = PhysicsBody::createBox(DEST_SIZE);
    physicsBody->setDynamic(false);
    physicsBody->setCategoryBitmask(PhysicsBitmask::DESTINATION);
    physicsBody->setContactTestBitmask(PhysicsBitmask::PLAYER);
    _destination->setPhysicsBody(physicsBody);
}

void StealthDetectionScene::initWalls(const ax::Vec2 &origin)
{
    const int cols = static_cast<int>((_director->getVisibleSize().x - 2 * FLOOR_POS.x) / WALL_SIZE.x);
    const int rows = static_cast<int>((_director->getVisibleSize().y - 2 * FLOOR_POS.y) / WALL_SIZE.y);

    const auto upperWall = createWall(cols, 1);
    upperWall->setPosition(origin.x + FLOOR_POS.x, origin.y + FLOOR_POS.y + static_cast<float>(rows - 1) * WALL_SIZE.y);
    addChild(upperWall);
    const auto lowerWall = createWall(cols, 1);
    lowerWall->setPosition(origin.x + FLOOR_POS.x, origin.y + FLOOR_POS.y);
    addChild(lowerWall);
    const auto leftWall = createWall(1, rows);
    leftWall->setPosition(origin.x + FLOOR_POS.x, origin.y + FLOOR_POS.y);
    addChild(leftWall);
    const auto rightWall = createWall(1, rows);
    rightWall->setPosition(origin.x + FLOOR_POS.x + static_cast<float>(cols - 1) * WALL_SIZE.x, origin.y + FLOOR_POS.y);
    addChild(rightWall);
    const auto lowerObstacle = createWall(1, 2);
    lowerObstacle->setPosition(origin.x + FLOOR_POS.x + 6 * WALL_SIZE.x, origin.y + FLOOR_POS.y + WALL_SIZE.y);
    addChild(lowerObstacle);
    const auto upperObstacle = createWall(1, 2);
    upperObstacle->setPosition(origin.x + FLOOR_POS.x + 13 * WALL_SIZE.x, origin.y + FLOOR_POS.y + static_cast<float>(rows - 3) * WALL_SIZE.y);
    addChild(upperObstacle);
}

void StealthDetectionScene::initResetButton(const ax::Vec2 &origin)
{
    const auto label = Label::createWithTTF("Reset", "fonts/arial.ttf", 24.0f);
    const auto item = MenuItemLabel::create(label, [this](Object*) { reset(); });
    const auto bg = DrawNode::create();
    bg->setContentSize(RESET_SIZE);
    bg->drawSolidRect(Vec2::ZERO, Vec2(RESET_SIZE), Color4F::ORANGE);
    item->addChild(bg, -1);
    item->setContentSize(RESET_SIZE);
    label->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    label->setPosition(RESET_SIZE.x / 2.0f, RESET_SIZE.y / 2.0f);
    const auto menu = Menu::create(item, nullptr);
    menu->setPosition(origin + RESET_POS);
    addChild(menu, 1);
}

void StealthDetectionScene::reset()
{
    resetPlayer();
    _successLbl->setVisible(false);
}

void StealthDetectionScene::onKeyPressed(const EventKeyboard::KeyCode keyCode)
{
    switch (keyCode)
    {
        case EventKeyboard::KeyCode::KEY_W: // UP
            if (_playerVelocity == Vec2::ZERO) {
                _playerVelocity = Vec2(0.0f, PLAYER_SPEED);
                _playerPhysicsBody->setVelocity(_playerVelocity);
            }
            break;

        case EventKeyboard::KeyCode::KEY_S: // DOWN
            if (_playerVelocity == Vec2::ZERO) {
                _playerVelocity = Vec2(0.0f, -PLAYER_SPEED);
                _playerPhysicsBody->setVelocity(_playerVelocity);
            }
            break;

        case EventKeyboard::KeyCode::KEY_A: // LEFT
            if (_playerVelocity == Vec2::ZERO) {
                _playerVelocity = Vec2(-PLAYER_SPEED, 0.0f);
                _playerPhysicsBody->setVelocity(_playerVelocity);
            }
            break;

        case EventKeyboard::KeyCode::KEY_D: // RIGHT
            if (_playerVelocity == Vec2::ZERO) {
                _playerVelocity = Vec2(PLAYER_SPEED, 0.0f);
                _playerPhysicsBody->setVelocity(_playerVelocity);
            }
            break;

        default:
            break;
        }
}

void StealthDetectionScene::onKeyReleased(const EventKeyboard::KeyCode keyCode)
{
    switch (keyCode)
    {
        case EventKeyboard::KeyCode::KEY_W: // UP
            if (_playerVelocity == Vec2(0.0f, PLAYER_SPEED)) {
                _playerVelocity = Vec2::ZERO;
                _playerPhysicsBody->setVelocity(_playerVelocity);
            }
            break;

        case EventKeyboard::KeyCode::KEY_S: // DOWN
            if (_playerVelocity == Vec2(0.0f, -PLAYER_SPEED)) {
                _playerVelocity = Vec2::ZERO;
                _playerPhysicsBody->setVelocity(_playerVelocity);
            }
            break;

        case EventKeyboard::KeyCode::KEY_A: // LEFT
            if (_playerVelocity == Vec2(-PLAYER_SPEED, 0.0f)) {
                _playerVelocity = Vec2::ZERO;
                _playerPhysicsBody->setVelocity(_playerVelocity);
            }
            break;

        case EventKeyboard::KeyCode::KEY_D: // RIGHT
            if (_playerVelocity == Vec2(PLAYER_SPEED, 0.0f)) {
                _playerVelocity = Vec2::ZERO;
                _playerPhysicsBody->setVelocity(_playerVelocity);
            }
            break;

        default:
            break;
        }
}

bool StealthDetectionScene::onContactBegin(const PhysicsContact &contact)
{
    if (contact.getShapeA()->getBody()->getCategoryBitmask() == PhysicsBitmask::GUARD_VISION ||
        contact.getShapeB()->getBody()->getCategoryBitmask() == PhysicsBitmask::GUARD_VISION)
    {
        resetPlayer();
    }
    else if (contact.getShapeA()->getBody()->getCategoryBitmask() == PhysicsBitmask::DESTINATION ||
             contact.getShapeB()->getBody()->getCategoryBitmask() == PhysicsBitmask::DESTINATION)
    {
        _successLbl->setVisible(true);
    }
    else if (contact.getShapeA()->getBody()->getCategoryBitmask() == PhysicsBitmask::WALL ||
             contact.getShapeB()->getBody()->getCategoryBitmask() == PhysicsBitmask::WALL)
    {
        return true;
    }

    return false;
}

void StealthDetectionScene::resetPlayer()
{
    scheduleOnce([this](float) { 
        _playerPhysicsBody->setEnabled(false);
        _playerPhysicsBody->setVelocity(Vec2::ZERO);
        _playerPhysicsBody->setAngularVelocity(0.0f);
        _playerPhysicsBody->resetForces();
        _player->setPosition(_director->getVisibleOrigin() + START_POS);
        _playerPhysicsBody->setEnabled(true);
    }, 0.0f, "resetPlayer");
}

bool StealthDetectionScene::onContactPresolve(const ax::PhysicsContact &contact) const
{
    if (contact.getShapeA()->getBody()->getCategoryBitmask() == PhysicsBitmask::WALL ||
        contact.getShapeB()->getBody()->getCategoryBitmask() == PhysicsBitmask::WALL)
    {
        _playerPhysicsBody->setVelocity(_playerVelocity);
    }

    return true;
}

DrawNode *StealthDetectionScene::createWall(const int cols, const int rows) const
{
    const auto wall = DrawNode::create();
    const auto& size = Vec2(static_cast<float>(cols) * WALL_SIZE.x, static_cast<float>(rows) * WALL_SIZE.y);
    wall->setContentSize(size);
    wall->drawSolidRect(Vec2::ZERO, Vec2(size), Color4F::GRAY);

    const auto physicsBody = PhysicsBody::createBox(size);
    physicsBody->setDynamic(false);
    physicsBody->getShape(0)->setFriction(0.0f);
    physicsBody->getShape(0)->setRestitution(0.0f);
    physicsBody->setCategoryBitmask(PhysicsBitmask::WALL);
    physicsBody->setCollisionBitmask(PhysicsBitmask::PLAYER);
    physicsBody->setContactTestBitmask(PhysicsBitmask::PLAYER);
    wall->setPhysicsBody(physicsBody);

    return wall;
}
