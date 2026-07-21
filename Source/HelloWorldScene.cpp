#include "HelloWorldScene.h"

using namespace ax;

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Scene::init() )
    {
        return false;
    }

    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    initScore(visibleSize, origin);
    initLives(visibleSize, origin);
    initPaddle(visibleSize, origin);
    initBall(visibleSize, origin);
    initEndLabel(visibleSize, origin);

    _lowerY = origin.y - Ball::Radius;

    initKeyboardListener();

    hardReset();

    return true;
}

void HelloWorld::initScore(const Size& visibleSize, const Vec2& origin)
{
    _score = 0;
    _scoreLabel = Label::createWithSystemFont("SCORE: 0", "arial", 20.0f);
    _scoreLabel->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
    _scoreLabel->setPosition(origin.x + 16.0f, origin.y + visibleSize.height - 16.0f);
    addChild(_scoreLabel, 1);
}

void HelloWorld::initLives(const Size& visibleSize, const Vec2& origin)
{
    _lives = 3;
    _livesLabel = Label::createWithSystemFont("LIVES: 3", "arial", 20.0f);
    _livesLabel->setAnchorPoint(Vec2::ANCHOR_TOP_RIGHT);
    _livesLabel->setPosition(origin.x + visibleSize.width - 16.0f, origin.y + visibleSize.height - 16.0f);
    addChild(_livesLabel, 1);
}

void HelloWorld::setScore(int value)
{
    _score = value;
    _scoreLabel->setString(StringUtils::format("SCORE: %d", value));
}

void HelloWorld::setLives(int value)
{
    _lives = value;
    _livesLabel->setString(StringUtils::format("LIVES: %d", value));
}

void HelloWorld::initPaddle(const Size& visibleSize, const Vec2& origin)
{
    _paddle = Paddle::create();
    _paddle->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    addChild(_paddle);
    _paddleStart = Vec2(origin.x + visibleSize.width / 2.0f, origin.y + 40.0f);
}

void HelloWorld::initBricks()
{
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    float xOffset = 34.0f;
    float yOffset = 380.0f;
    float spacing = 8.0f;
    for (int col = 0; col < 10; ++col) {
        for (int row = 4; row > -1; --row) {
            auto brick = Brick::create(row > 2);
            brick->setPosition(origin.x + xOffset + col * (spacing + Brick::BrickSize.width),
                origin.y + yOffset + row * (spacing + Brick::BrickSize.height));
            addChild(brick);
            _bricks.push_back(brick);
        }
    }
}

void HelloWorld::initBall(const Size& visibleSize, const Vec2& origin)
{
    _ball = Ball::create();
    _ball->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    addChild(_ball);
    _ballStart = Vec2(origin.x + visibleSize.width / 2.0f, _paddleStart.y + Ball::Radius);
}

void HelloWorld::initKeyboardListener()
{
    _keyListener = EventListenerKeyboard::create();
    _keyListener->onKeyPressed = [this](EventKeyboard::KeyCode key, Event *_) {
        onKeyPressed(key);
    };
    _keyListener->onKeyReleased = [this](EventKeyboard::KeyCode key, Event *_) {
        onKeyReleased(key);
    };
    _eventDispatcher->addEventListenerWithSceneGraphPriority(_keyListener, this);
}

void HelloWorld::onKeyPressed(EventKeyboard::KeyCode key)
{
    switch (key)
    {
    case EventKeyboard::KeyCode::KEY_LEFT_ARROW:
    case EventKeyboard::KeyCode::KEY_A:
        _paddle->goLeft();
        break;

    case EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
    case EventKeyboard::KeyCode::KEY_D:
        _paddle->goRight();
        break;
    
    case EventKeyboard::KeyCode::KEY_SPACE:
        if (_isResting) {
            _isResting = false;
        }
        break;
    
    case EventKeyboard::KeyCode::KEY_R:
        if (_endLabel->isVisible()) {
            hardReset();
        }
        break;
    
    default:
        break;
    }
}

void HelloWorld::onKeyReleased(EventKeyboard::KeyCode key)
{
    switch (key)
    {
    case EventKeyboard::KeyCode::KEY_LEFT_ARROW:
    case EventKeyboard::KeyCode::KEY_A:
        if (_paddle->getDirection() == -1)
            _paddle->stop();
        break;

    case EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
    case EventKeyboard::KeyCode::KEY_D:
        if (_paddle->getDirection() == 1)
            _paddle->stop();
        break;
    
    default:
        break;
    }
}

void HelloWorld::update(float dt)
{
    _paddle->update(dt);
    if (_isResting)
        _ball->setPositionX(_paddle->getPositionX());
    else {
        _ball->update(dt);
        if (_ball->getPositionY() <= _lowerY) {
            setLives(_lives - 1);
            if (_lives == 0) {
                showResult(false);
            }
            else {
                softReset();
            }
        }
        else {
            checkBrickCollision();
            checkPaddleCollision();
        }
    }
}

void HelloWorld::softReset()
{
    _paddle->setPosition(_paddleStart);
    _ball->setPosition(_ballStart);
    _ball->resetVelocity();
    _isResting = true;
}

void HelloWorld::hardReset()
{
    _endLabel->setVisible(false);
    setScore(0);
    setLives(3);
    clearBricks();
    initBricks();
    softReset();
    scheduleUpdate();
}

void HelloWorld::clearBricks()
{
    for (auto it = _bricks.begin(); it != _bricks.end(); ) {
        removeChild(*it);
        it = _bricks.erase(it);
    }
}

void HelloWorld::checkBrickCollision()
{
    for (auto it = _bricks.begin(); it != _bricks.end(); it++) {
        if ((*it)->getBoundingBox().intersectsCircle(_ball->getPosition(), Ball::Radius)) {
            _ball->reverseVelocityY();
            auto brick = *it;
            if (brick->hit()) {
                setScore(_score + (brick->isStrong() ? 20 : 10));
                removeChild(*it);
                _bricks.erase(it);
            }
            break;
        }
    }

    if (_bricks.empty())
        showResult(true);
}

void HelloWorld::checkPaddleCollision()
{
    if (_paddle->getBoundingBox().intersectsCircle(_ball->getPosition(), Ball::Radius)) {
        float vx = 300.f * (_ball->getPositionX() - _paddle->getPositionX()) / (Paddle::PaddleSize.width / 2.0f);
        _ball->setVelocityX(vx);
    }
}

void HelloWorld::initEndLabel(const ax::Size& visibleSize, const ax::Vec2& origin)
{
    _endLabel = Label::createWithSystemFont("GAME OVER", "arial", 28.0f);
    _endLabel->setPosition(origin.x + visibleSize.width / 2.0f, origin.y + visibleSize.height / 2.0f);
    _endLabel->setVisible(false);
    addChild(_endLabel, 2);
}

void HelloWorld::showResult(bool win)
{
    unscheduleUpdate();
    _endLabel->setString(win ? "YOU WIN" : "GAME OVER");
    _endLabel->setVisible(true);
}