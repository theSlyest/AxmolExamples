#include "TeleporterScene.h"

USING_NS_AX;

const Color4B TeleporterScene::BLOCK_COLOR = Color4B(0x35, 0x35, 0x35, 255);

// on "init" you need to initialize your instance
bool TeleporterScene::init()
{
    if (!initWithPhysics())
        return false;

    const auto world = getPhysicsWorld();
    world->setGravity(GRAVITY);
    world->setSubsteps(2);

    const Vec2& size = Director::getInstance()->getVisibleSize();
    const Vec2& origin = Director::getInstance()->getVisibleOrigin();

    const auto background = LayerColor::create(Color4B(0xb7, 0xce, 0xeb, 255));
    addChild(background);

    initLevel(origin, size);
    initPlayer(origin);

    // Keyboard listener
    const auto keyListener = EventListenerKeyboard::create();
    keyListener->onKeyPressed = [this](const EventKeyboard::KeyCode key, Event *) { onKeyPressed(key); };
    _eventDispatcher->addEventListenerWithSceneGraphPriority(keyListener, this);

    // Touch/Mouse listener
    const auto touchListener = EventListenerTouchOneByOne::create();
    touchListener->onTouchBegan = [this](Touch*, Event*) { return onTouchBegan(); };
    touchListener->onTouchEnded = [this](const Touch* touch, Event*) { onTouchEnded(touch); };
    _eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);

    // Collision listener
    const auto contactListener = EventListenerPhysicsContact::create();
    contactListener->onContactBegin = [this](PhysicsContact &) { return onContactBegin(); };
    _eventDispatcher->addEventListenerWithSceneGraphPriority(contactListener, this);

    return true;
}

Node *TeleporterScene::createBlock(const Vec2 &size)
{
    auto block = LayerColor::create(BLOCK_COLOR, size.width, size.height);
    const auto body = PhysicsBody::createBox(size);
    body->setDynamic(false);
    body->setCategoryBitmask(BLOCK);
    body->setCollisionBitmask(PLAYER);
    body->setContactTestBitmask(BALL);
    body->getShape(0)->setRestitution(0.0f);
    block->setPhysicsBody(body);
    return block;
}

void TeleporterScene::initLevel(const Vec2& origin, const Vec2& size)
{
    constexpr float xUnit = 80.0f;
    constexpr float yUnit = 60.0f;

    const auto left = createBlock(Vec2(1.0f, size.height));
    left->setPosition(origin.x - 1.0f, origin.y);
    addChild(left);

    const auto first = createBlock(Vec2(2 * xUnit, yUnit));
    addChild(first);

    const auto b0 = createBlock(Vec2(xUnit, yUnit));
    b0->setPosition(origin.x + 3 * xUnit, origin.y + 2 * yUnit);
    addChild(b0);

    const auto b1 = createBlock(Vec2(xUnit, yUnit));
    b1->setPosition(origin.x + 5 * xUnit, origin.y);
    addChild(b1);

    const auto b2 = createBlock(Vec2(xUnit, 4 * yUnit));
    b2->setPosition(origin.x + 6 * xUnit, origin.y);
    addChild(b2);

    const auto b3 = createBlock(Vec2(2 * xUnit, yUnit));
    b3->setPosition(origin.x + 8 * xUnit, origin.y + 6 * yUnit);
    addChild(b3);

    const auto b4 = createBlock(Vec2(xUnit, yUnit));
    b4->setPosition(origin.x + 12 * xUnit, origin.y + 5 * yUnit);
    addChild(b4);

    const auto last = createBlock(Vec2(xUnit, 9 * yUnit));
    last->setPosition(origin.x + size.width - xUnit, origin.y);
    addChild(last);

    const auto right = createBlock(Vec2(1.0f, size.height));
    right->setPosition(origin.x + size.width, origin.y);
    addChild(right);

    const auto bottom = createBlock(Vec2(size.width, 1.0f));
    bottom->setPosition(origin.x, origin.y - 1.0f);
    addChild(bottom);
}

void TeleporterScene::initBall()
{
    _ball = Sprite::create("ball.png");
    _ball->setScale(BALL_SCALE);
    _ball->setVisible(false);
    addChild(_ball);

    const auto body = PhysicsBody::createBox(_ball->getContentSize());
    body->setDynamic(true);
    body->setRotationEnable(false);
    body->setCategoryBitmask(BALL);
    body->setContactTestBitmask(BLOCK);
    body->getShape(0)->setRestitution(0.0f);
    body->setEnabled(false);
    _ball->setPhysicsBody(body);
}

void TeleporterScene::initPlayer(const Vec2& origin)
{
    constexpr Rect frameRect(0.0f, PLAYER_SIZE * 11, PLAYER_SIZE, PLAYER_SIZE);
    _player = Sprite::create("spritesheet.png", frameRect);
    _player->setPosition(origin.x + 80.0f, origin.y + 85.0f);
    addChild(_player);

    const auto body = PhysicsBody::createBox(Vec2(PLAYER_SIZE, PLAYER_SIZE));
    body->setDynamic(true);
    body->setRotationEnable(false);
    body->setCategoryBitmask(PLAYER);
    body->setCollisionBitmask(BLOCK);
    body->getShape(0)->setRestitution(0.0f);
    _player->setPhysicsBody(body);

    initBall();
}

void TeleporterScene::onKeyPressed(EventKeyboard::KeyCode key)
{
    if (key == EventKeyboard::KeyCode::KEY_SPACE) {
        if (!_ball->isVisible())
            return;

        scheduleOnce([this](float) {
            hideBall();
            _player->setPosition(_ball->getPosition());
        }, 0.0f, KEY_TELEPORT);
    }
}

bool TeleporterScene::onTouchBegan()
{
    _isHold = false;
    scheduleOnce([this](float) {
        _isHold = true;
        _isSecondTap = false;
    }, HOLD_DURATION, KEY_HOLD);

    if (_isSecondTap) {
        unschedule(KEY_SINGLE_TAP);
    }

    return true;
}

void TeleporterScene::onTouchEnded(const Touch *touch)
{
    unschedule(KEY_HOLD);

    const Vec2 location = touch->getLocation();

    if (_isHold) {
        _isSecondTap = false;
        AXLOGD("DETECTED HOLD");
        throwBall(location, 3 * 75.0f, GRAVITY);
        return;
    }

    if (_isSecondTap) {
        _isSecondTap = false;
        AXLOGD("DETECTED DOUBLE TAP");
        throwBall(location, 3 * 90.0f, GRAVITY * 0.7f);
        return;
    }

    _isSecondTap = true;
    scheduleOnce([this, location](float) {
        _isSecondTap = false;
        AXLOGD("DETECTED SINGLE TAP");
        throwBall(location, 3 * 60.0f, GRAVITY * 1.3f);
    }, DOUBLE_TAP_INTERVAL, KEY_SINGLE_TAP);
}

bool TeleporterScene::onContactBegin()
{
    scheduleOnce([this](float) { hideBall(); }, 0.0f, KEY_HIDE_BALL);
    return false;
}

void TeleporterScene::hideBall() const
{
    _ball->setVisible(false);
    _ball->getPhysicsBody()->setEnabled(false);
    getPhysicsWorld()->setGravity(GRAVITY);
}

void TeleporterScene::throwBall(const Vec2 &position, const float speed, const Vec2 &gravity)
{
    const Vec2 ballPos = _player->getPosition() + Vec2(0.0f, PLAYER_SIZE);
    const Vec2 velocity = (position - ballPos).getNormalized() * speed;
    getPhysicsWorld()->setGravity(gravity);

    scheduleOnce([this, ballPos, velocity](float) {
        _ball->setPosition(ballPos);
        _ball->setVisible(true);
        const auto body = _ball->getPhysicsBody();
        body->setEnabled(true);
        body->setVelocity(velocity);
    }, 0.0f, KEY_THROW_BALL);
}
