#include "ClonesScene.h"

#include "Constants.h"
#include "Goal.h"
#include "Platform.h"

USING_NS_AX;

// on "init" you need to initialize your instance
bool ClonesScene::init()
{
    //////////////////////////////
    // 1. super init first
    if (!Scene::initWithPhysics())
    {
        return false;
    }

    _physicsWorld->setGravity(Vec2(0.0f, -480.0f));
    _physicsWorld->setSubsteps(4);

    auto visibleSize = _director->getVisibleSize();
    Vec2 origin = _director->getVisibleOrigin();

    initFloor(visibleSize, origin);
    initGameOverLayer(visibleSize, origin);

    _goalReachedLabel = Label::createWithTTF("Reached: 0", "fonts/arial.ttf", 20.0f);
    _goalReachedLabel->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
    _goalReachedLabel->setPosition(origin.x + 20.0f, origin.y + visibleSize.height - 20.0f);
    addChild(_goalReachedLabel, 1);

    _timerLabel = Label::createWithTTF("Remaining : 30", "fonts/arial.ttf", 20.0f);
    _timerLabel->setAnchorPoint(Vec2::ANCHOR_TOP_RIGHT);
    _timerLabel->setPosition(origin.x + visibleSize.width - 20.0f, origin.y + visibleSize.height - 20.0f);
    addChild(_timerLabel, 1);

    _goal = Goal::create();
    _goal->setPosition(Vec2(origin.x + visibleSize.width - _goal->getContentSize().width * _goal->getScaleX() / 2.0f,
                            origin.y + _goal->getContentSize().height * _goal->getScaleY() / 2.0f + 20.0f)); // Position the goal at the bottom right corner
    addChild(_goal);

    _keyboardListener = EventListenerKeyboard::create();
    _keyboardListener->onKeyPressed = [this](const EventKeyboard::KeyCode key, Event* event) { return onKeyPressed(key); };
    _keyboardListener->onKeyReleased = [this](const EventKeyboard::KeyCode key, Event* event) { return onKeyReleased(key); };
    _eventDispatcher->addEventListenerWithSceneGraphPriority(_keyboardListener, this);

    _contactListener = EventListenerPhysicsContact::create();
    _contactListener->onContactBegin = [this](const PhysicsContact& contact) { return onContactBegin(contact); };
    _eventDispatcher->addEventListenerWithSceneGraphPriority(_contactListener, this);

    resetGame();

    return true;
}

void ClonesScene::initFloor(const Size &visibleSize, const Vec2 &origin)
{
    auto floor = Platform::create(visibleSize.width);
    floor->setPosition(Vec2(visibleSize.width / 2.0f + origin.x, origin.y + floor->getContentSize().height / 2.0f * floor->getScaleY()));
    addChild(floor, 0);
}

void ClonesScene::initGameOverLayer(const Size &visibleSize, const Vec2 &origin)
{
    _gameOverLayer = Layer::create();
    _gameOverLayer->setContentSize(visibleSize);
    _gameOverLayer->setPosition(origin);
    _gameOverLayer->setVisible(false);

    const auto gameOverLabel = Label::createWithTTF("You Win", "fonts/Marker Felt.ttf", 48.0f);
    gameOverLabel->setPosition(Vec2(origin.x + visibleSize.width / 2.0f, origin.y + visibleSize.height / 2.0f));
    _gameOverLayer->addChild(gameOverLabel);

    const auto restartButton = MenuItemFont::create("Restart", [this](Object *sender)
                                              { resetGame(); });
    MenuItemFont::setFontSize(24);
    restartButton->setPosition(Vec2(origin.x + visibleSize.width / 2.0f, origin.y + visibleSize.height / 2.0f - 40.0f));
    const auto menu = Menu::create(restartButton, nullptr);
    menu->setPosition(Vec2::ZERO);
    _gameOverLayer->addChild(menu);

    addChild(_gameOverLayer, 1);
}

void ClonesScene::resetGame()
{
    for (Player* clone: _clones) {
        removeChild(clone, true);
    }
    _clones.clear();
    _goalReachedCount = 0;
    updateGoalReachedLabel();
    _timer = 29;
    updateTimerLabel();
    _gameOverLayer->setVisible(false);
    _xMove = XMove::None;
    _isJumping = false;
    _isPlaying = true;
    _player = Player::create();
    applyInitialPlayerPosition(_player);
    addChild(_player);
    scheduleReplay();
    scheduleUpdate();
}

bool ClonesScene::onKeyPressed(EventKeyboard::KeyCode key)
{
    if (!_isPlaying)
        return false;

    constexpr bool isPressed = true;
    switch (key)
    {
    case EventKeyboard::KeyCode::KEY_LEFT_ARROW:
    case EventKeyboard::KeyCode::KEY_A:
        _recorder.record(static_cast<int>(key), isPressed);
        if (_player != nullptr)
            _player->startMovingLeft();
        return true;

    case EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
    case EventKeyboard::KeyCode::KEY_D:
        _recorder.record(static_cast<int>(key), isPressed);
        if (_player != nullptr)
            _player->startMovingRight();
        return true;

    case EventKeyboard::KeyCode::KEY_UP_ARROW:
    case EventKeyboard::KeyCode::KEY_SPACE:
        _recorder.record(static_cast<int>(key), isPressed);
        if (_player != nullptr)
            _player->jump();
        return true;

    default:
        return false;
    }
}

bool ClonesScene::onKeyReleased(EventKeyboard::KeyCode key)
{
    if (!_isPlaying)
        return false;

    constexpr bool isPressed = false;
    switch (key)
    {
    case EventKeyboard::KeyCode::KEY_LEFT_ARROW:
    case EventKeyboard::KeyCode::KEY_A:
        _recorder.record(static_cast<int>(key), isPressed);
        if (_player != nullptr)
            _player->stopMovingLeft();
        return true;

    case EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
    case EventKeyboard::KeyCode::KEY_D:
        _recorder.record(static_cast<int>(key), isPressed);
        if (_player != nullptr)
            _player->stopMovingRight();
        return true;

    default:
        return false;
    }
}

bool ClonesScene::onContactBegin(const PhysicsContact &contact)
{

    const auto bodyA = contact.getShapeA()->getBody();
    const auto bodyB = contact.getShapeB()->getBody();

    PhysicsBody* playerBody = bodyA->getCategoryBitmask() == BITMASK_PLAYER ? bodyA : bodyB;
    playerBody->setVelocity(Vec2(playerBody->getVelocity().x, 0.0f));

    auto *player = dynamic_cast<Player *>(playerBody->getNode());

    if (player != nullptr) {
        // Goal reached
        if (bodyA->getCategoryBitmask() == BITMASK_GOAL || bodyB->getCategoryBitmask() == BITMASK_GOAL) {
            if (player == _player)
                _player = nullptr;
            else
                _currentClone = nullptr;

            player->removeFromParentAndCleanup(true);
            _goalReachedCount++;
            updateGoalReachedLabel();
            if (_goalReachedCount == 5) {
                endGame();
            }
            return false;
        }

        Vec2 normal = contact.getContactData()->normal;
        if (normal.y < -0.7f) {
            player = dynamic_cast<Player *>(playerBody->getNode());
            player->land();
        }
    }

    return true;
}

void ClonesScene::scheduleReplay()
{
    _recorder.start();
    schedule([this](float dt)
            {
                _timer--;
                if (_timer == -1) {
                    if (_currentClone != nullptr)
                        _clones.push_back(_currentClone);

                    _timer = 29;
                    _recorder.restart();
                    _currentClone = Player::create(true);
                    applyInitialPlayerPosition(_currentClone);
                    addChild(_currentClone);
                }
                updateTimerLabel();
            },
            1.0f, REC_KEY);
}

void ClonesScene::unscheduleReplay()
{
    unschedule(REC_KEY);
}

void ClonesScene::endGame() 
{
    _isPlaying = false;
    unscheduleUpdate();
    unscheduleReplay();
    _gameOverLayer->setVisible(true);
}

void ClonesScene::update(float dt)
{
    if (_currentClone != nullptr)
    {
        auto record = _recorder.readRecord();
        if (record.first) {
            replayInput((EventKeyboard::KeyCode) record.second.keyCode, record.second.isPressed);
        }
    }
}

void ClonesScene::replayInput(EventKeyboard::KeyCode keyCode, bool isPressed)
{
    if (isPressed) {
        switch (keyCode)
        {
        case EventKeyboard::KeyCode::KEY_LEFT_ARROW:
        case EventKeyboard::KeyCode::KEY_A:
            _currentClone->startMovingLeft();
            break;

        case EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
        case EventKeyboard::KeyCode::KEY_D:
            _currentClone->startMovingRight();
            break;

        case EventKeyboard::KeyCode::KEY_UP_ARROW:
        case EventKeyboard::KeyCode::KEY_SPACE:
            _currentClone->jump();
            break;

        default:
            break;
        }
    }
    else {
        switch (keyCode)
        {
        case EventKeyboard::KeyCode::KEY_LEFT_ARROW:
        case EventKeyboard::KeyCode::KEY_A:
            _currentClone->stopMovingLeft();
            break;

        case EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
        case EventKeyboard::KeyCode::KEY_D:
            _currentClone->stopMovingRight();
            break;

        default:
            break;
        }
    }
}

void ClonesScene::updateGoalReachedLabel()
{
    _goalReachedLabel->setString(StringUtils::format("Reached: %d", _goalReachedCount));
}

void ClonesScene::updateTimerLabel()
{
    _timerLabel->setString(StringUtils::format("Remaining: %d", _timer));
}

void ClonesScene::applyInitialPlayerPosition(Player* player)
{
    auto origin = _director->getVisibleOrigin();
    player->setPosition(origin.x + player->getContentSize().width * player->getScale() / 2.0f,
                        origin.y + player->getContentSize().height * player->getScale() / 2.0f + 20.0f); // Reset to initial position

}