#include "EscortingScene.h"

USING_NS_AX;

// on "init" you need to initialize your instance
bool EscortingScene::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Scene::init() )
        return false;

    _visibleSize = Director::getInstance()->getVisibleSize();
    _origin = Director::getInstance()->getVisibleOrigin();

    initDestination();
    initEscort();
    initPlayer();
    initComplete();
    initKeyboardListener();

    scheduleUpdate();

    _state = GameState::IDLE;

    return true;
}

void EscortingScene::updateCharacterPosition(Character *character, const float dt) const
{
    Vec2 position = character->nextPosition(dt);
    position.x = std::max(position.x, _origin.x);
    position.x = std::min(position.x, _origin.x + _visibleSize.width - Player::SIZE.width);
    position.y = std::max(position.y, _origin.y);
    position.y = std::min(position.y, _origin.y + _visibleSize.height - Player::SIZE.height);
    character->setPosition(position);
}

void EscortingScene::update(const float dt)
{
    updateCharacterPosition(_player, dt);
    if (_state == GameState::ESCORTING) {
        if (playerEscortDistance() >= 80.0f) {
            _state = GameState::FOLLOWING;
            _recorder.startReading();
            updateEscortPosition(dt);
        }
    }
    else if (_state == GameState::FOLLOWING) {
        if (_player->getBoundingBox().intersectsRect(_destination->getBoundingBox())) {
            _state = GameState::COMPLETE;
            escortToDestination();
        }
        else {
            updateEscortPosition(dt);
        }
    }
}

void EscortingScene::updateEscortPosition(const float dt)
{
    if (Vec2 direction; _recorder.read(direction)) {
        _npc->setDirection(direction);
    }
    updateCharacterPosition(_npc, dt);
}

void EscortingScene::initDestination()
{
    _destination = Destination::create();
    _destination->setPosition(_origin.x + _visibleSize.width - Destination::SIZE.width - OFFSET, 
        _origin.y + _visibleSize.height - Destination::SIZE.height - OFFSET);
    _destination->setVisible(false);
    addChild(_destination);
}

void EscortingScene::initPlayer()
{
    _player = Player::create();
    _player->setPosition(_origin.x + OFFSET, _origin.y + _visibleSize.height - Player::SIZE.height - OFFSET);
    addChild(_player);
}

void EscortingScene::initComplete()
{
    _completeLabel = Label::createWithTTF("Escort Complete", "fonts/arial.ttf", 64.0f);
    _completeLabel->setPosition(_origin.x + _visibleSize.width / 2.0f, _origin.y + _visibleSize.height / 2.0f);
    _completeLabel->setVisible(false);
    addChild(_completeLabel, 1);
}

void EscortingScene::initEscort()
{
    _npc = Npc::create();
    _npc->setPosition(_origin.x + OFFSET, _origin.y + OFFSET);
    addChild(_npc);
}

void EscortingScene::initKeyboardListener()
{
    _keyboardListener = EventListenerKeyboard::create();
    _keyboardListener->onKeyPressed = [this](EventKeyboard::KeyCode key, Event *) { onKeyPressed(key); };
    _keyboardListener->onKeyReleased = [this](EventKeyboard::KeyCode key, Event *) { onKeyReleased(key); };
    _eventDispatcher->addEventListenerWithSceneGraphPriority(_keyboardListener, this);
}

void EscortingScene::onKeyPressed(const EventKeyboard::KeyCode key)
{
    switch (key) {
        case EventKeyboard::KeyCode::KEY_W:
        case EventKeyboard::KeyCode::KEY_CAPITAL_W:
            movePlayer('W');
            break;

        case EventKeyboard::KeyCode::KEY_S:
        case EventKeyboard::KeyCode::KEY_CAPITAL_S:
            movePlayer('S');
            break;

        case EventKeyboard::KeyCode::KEY_A:
        case EventKeyboard::KeyCode::KEY_CAPITAL_A:
            movePlayer('A');
            break;

        case EventKeyboard::KeyCode::KEY_D:
        case EventKeyboard::KeyCode::KEY_CAPITAL_D:
            movePlayer('D');
            break;

        case EventKeyboard::KeyCode::KEY_E:
        case EventKeyboard::KeyCode::KEY_CAPITAL_E:
            if (_state == GameState::IDLE && tryTalk()) {
                _state = GameState::ESCORTING;
                _destination->setVisible(true);
                if (_player->isMoving()) {
                    _recorder.startRecording();
                    _recorder.record(_player->getDirection());
                }
            }
            break;
    }
}

void EscortingScene::movePlayer(const char key)
{
    if (!_player->isMoving()) {
        const Vec2& direction = Directions.at(key);
        _player->setDirection(direction);
        if (_state == GameState::ESCORTING) {
            _recorder.startRecording();
            _recorder.record(direction);
        }
        else if (_state == GameState::FOLLOWING) {
            _recorder.record(direction);
        }
    }
}

void EscortingScene::onKeyReleased(const EventKeyboard::KeyCode key)
{
    switch (key) {
        case EventKeyboard::KeyCode::KEY_W:
        case EventKeyboard::KeyCode::KEY_CAPITAL_W:
            if (_player->getDirection() == Directions.at('W')) {
                stopPlayer();
            }
            break;

        case EventKeyboard::KeyCode::KEY_S:
        case EventKeyboard::KeyCode::KEY_CAPITAL_S:
            if (_player->getDirection() == Directions.at('S')) {
                stopPlayer();
            }
            break;

        case EventKeyboard::KeyCode::KEY_A:
        case EventKeyboard::KeyCode::KEY_CAPITAL_A:
            if (_player->getDirection() == Directions.at('A')) {
                stopPlayer();
            }
            break;

        case EventKeyboard::KeyCode::KEY_D:
        case EventKeyboard::KeyCode::KEY_CAPITAL_D:
            if (_player->getDirection() == Directions.at('D')) {
                stopPlayer();
            }
            break;
    }
}

void EscortingScene::stopPlayer()
{
    _player->setDirection(Vec2::ZERO);
    if (_state == GameState::ESCORTING || _state == GameState::FOLLOWING) {
        _recorder.record(Vec2::ZERO);
    }
}

bool EscortingScene::tryTalk() { return playerEscortDistance() <= TALK_DISTANCE; }

void EscortingScene::escortToDestination()
{
    Vec2 escortCenter = _npc->getPosition() + Vec2(_npc->getContentSize() / 2.0f);
    Vec2 destinationCenter = _destination->getPosition() + Vec2(_destination->getContentSize() / 2.0f);
    float distance = escortCenter.getDistance(destinationCenter);
    float duration = distance / Character::SPEED;

    auto moveTo = MoveTo::create(duration, destinationCenter - Vec2(_npc->getContentSize()) / 2.0f);
    auto complete = CallFunc::create([this]() { _completeLabel->setVisible(true); });
    auto sequence = Sequence::create(moveTo, complete, nullptr);

    _npc->runAction(sequence);
}

float EscortingScene::playerEscortDistance()
{
    Vec2 playerCenter = _player->getPosition() + Vec2(_player->getContentSize() / 2.0f);
    Vec2 escortCenter = _npc->getPosition() + Vec2(_npc->getContentSize() / 2.0f);
    return playerCenter.getDistance(escortCenter);
}
