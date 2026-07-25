#include "CrossTheRoadScene.h"

USING_NS_AX;

// on "init" you need to initialize your instance
bool CrossTheRoadScene::init()
{
    if ( !Scene::init() )
        return false;

    const auto& visibleSize = Director::getInstance()->getVisibleSize();
    const Vec2& origin = Director::getInstance()->getVisibleOrigin();

    // Score label
    _scoreLabel = Label::createWithTTF("Score: 0", "fonts/arial.ttf", 24.0f);
    _scoreLabel->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
    _scoreLabel->setPosition(Vec2(origin.x + 20.0f, origin.y + visibleSize.height - 50.0f));
    addChild(_scoreLabel, 1);

    _discreteWidth = visibleSize.width / BASE_LENGTH;
    _discreteHeight = visibleSize.height / BASE_LENGTH;

    // Safe zone
    constexpr int safeHeight = 1;
    _safeZone = BaseBox::create(BoxType::SafeZone, _discreteWidth, safeHeight);
    _safeZone->setDiscretePosition(0, _discreteHeight - safeHeight);
    addChild(_safeZone);

    // Player
    constexpr int playerSize = 1;
    _player = BaseBox::create(BoxType::Player, playerSize, playerSize);
    addChild(_player);

    _keyboardListener = EventListenerKeyboard::create();
    _keyboardListener->onKeyPressed = [this](const EventKeyboard::KeyCode key, Event*) { onKeyPressed(key); };
    _eventDispatcher->addEventListenerWithSceneGraphPriority(_keyboardListener, this);

    _lanes.assign(14, {});

    _director->getEventDispatcher()->addCustomEventListener("obstacleMove", [this](EventCustom* event) { onObstacleMove(event); });
    _director->getEventDispatcher()->addCustomEventListener("obstacleOut", [this](EventCustom* event) { onObstacleOut(event); });
    resetGame();

    return true;
}

void CrossTheRoadScene::resetGame()
{
    unschedule("spawObstacle");

    _score = 0;
    updateScoreLabel();
    _player->setDiscretePosition(_discreteWidth / 2, 0);
    for (auto& lane : _lanes) {
        for (BaseBox* obstacle : lane) {
            removeChild(obstacle, true);
        }
        lane.clear();
    }

    schedule([this](float _) { spawnObstacle(); }, 1.0f, "spawObstacle");
}

void CrossTheRoadScene::onKeyPressed(EventKeyboard::KeyCode key)
{
    int pos;
    switch (key) {
        case EventKeyboard::KeyCode::KEY_A: // Left
        case EventKeyboard::KeyCode::KEY_LEFT_ARROW: // Left
            pos = _player->getDiscretePositionX();
            if (pos > 0) {
                _player->setDiscretePosition(pos - 1, _player->getDiscretePositionY());
            }
            break;

        case EventKeyboard::KeyCode::KEY_D: // Right
        case EventKeyboard::KeyCode::KEY_RIGHT_ARROW: // Right
            pos = _player->getDiscretePositionX();
            if (pos < _discreteWidth - 1) {
                _player->setDiscretePosition(pos + 1, _player->getDiscretePositionY());
            }
            break;

        case EventKeyboard::KeyCode::KEY_W: // Up
        case EventKeyboard::KeyCode::KEY_UP_ARROW: // Up
            pos = _player->getDiscretePositionY();
            if (pos < _discreteHeight - 1) {
                _player->setDiscretePosition(_player->getDiscretePositionX(), pos + 1);
                laneChanged();
            }
            break;

        case EventKeyboard::KeyCode::KEY_S: // Down
        case EventKeyboard::KeyCode::KEY_DOWN_ARROW: // Down
            pos = _player->getDiscretePositionY();
            if (pos > 0) {
                _player->setDiscretePosition(_player->getDiscretePositionX(), pos - 1);
                laneChanged();
            }
            break;
    }
}

void CrossTheRoadScene::spawnObstacle()
{
    const int lane = RandomHelper::random_int(0, _lanesCount - 1);
    Obstacle* obstacle = Obstacle::create(lane);
    addChild(obstacle);
    _lanes[lane].push_back(obstacle);
}

bool CrossTheRoadScene::checkIntersection(const Obstacle* obstacle) const
{
    if (obstacle->getLane() != _player->getDiscretePositionY() - 1) 
        return false;
    
    if (obstacle->getBoundingBox().intersectsRect(_player->getBoundingBox()))
        return true;

    return false;
}

void CrossTheRoadScene::onObstacleOut(const EventCustom* event)
{
    auto* out = static_cast<Obstacle*>(event->getUserData());
    _lanes[out->getLane()].remove(out);
    removeChild(out, true);
}

void CrossTheRoadScene::onObstacleMove(const EventCustom* event)
{
    if (auto* moved = static_cast<Obstacle*>(event->getUserData()); checkIntersection(moved))
        resetGame();
}

void CrossTheRoadScene::laneChanged() 
{
    const int lane = _player->getDiscretePositionY() - 1;
    if (lane == -1)
        return;

    if (lane == _lanesCount)
    {
        _score++;
        updateScoreLabel();
        _player->setDiscretePosition(_discreteWidth / 2, 0);
        return;
    }

    for (const Obstacle* obstacle : _lanes[lane]) {
        if (checkIntersection(obstacle))
        {
            resetGame();
            break;
        }
    }
}

void CrossTheRoadScene::updateScoreLabel() const
{
    _scoreLabel->setString(StringUtils::format("Score: %d", _score));
}
