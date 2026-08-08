#include "SignalLockScene.h"

#include "Director.h"
#include "EventDispatcher.h"

USING_NS_AX;

// on "init" you need to initialize your instance
bool SignalLockScene::init()
{
    if ( !Scene::init() )
        return false;

    const Vec2& size = Director::getInstance()->getVisibleSize();
    const Vec2& origin = Director::getInstance()->getVisibleOrigin();

    const auto bg = LayerColor::create(Color4B(0x0b, 0x0f, 0x1a, 255));
    addChild(bg);

    // Score
    _scoreLabel = Label::createWithTTF("Score: 0", "fonts/arial.ttf", 24.0f);
    _scoreLabel->setTextColor(UI_COLOR);
    _scoreLabel->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
    _scoreLabel->setPosition(origin.x + LABELS_OFFSET, origin.y + size.y - LABELS_OFFSET);
    addChild(_scoreLabel);

    // Time
    _timeLabel = Label::createWithTTF("Time: 00", "fonts/arial.ttf", 24.0f);
    _timeLabel->setTextColor(UI_COLOR);
    _timeLabel->setAnchorPoint(Vec2::ANCHOR_TOP_RIGHT);
    _timeLabel->setPosition(origin.x + size.x - LABELS_OFFSET, origin.y + size.y - LABELS_OFFSET);
    addChild(_timeLabel);

    // Dial
    _dial = SignalDial::create();
    _dial->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    _dial->setPosition(origin.x + size.x / 2.0f, 40.0f);
    addChild(_dial);
    _velocity = SignalDial::getSpeed();

    // Help
    const auto help = Label::createWithTTF("Hold LEFT or RIGHT to tune", "fonts/arial.ttf", 24.0f);
    help->setPosition(origin.x + size.x / 2.0f, origin.y + 240.0f);
    addChild(help);

    // Screen
    _screen = SignalScreen::create();
    _screen->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    _screen->setPosition(origin.x + size.x * 0.5f, origin.x + 288.0f);
    addChild(_screen);

    initEndLayer(size);

    // Keyboard listener
    _keyListener = EventListenerKeyboard::create();
    _keyListener->onKeyPressed = [this](const EventKeyboard::KeyCode key, Event *) { onKeyPressed(key); };
    _keyListener->onKeyReleased = [this](const EventKeyboard::KeyCode key, Event *) { onKeyReleased(key); };
    _eventDispatcher->addEventListenerWithSceneGraphPriority(_keyListener, this);

    reset();

    return true;
}

void SignalLockScene::update(const float dt)
{
    if (!_isPlaying)
        return;

    _timer += dt;
    if (_timer >= 1.0f) {
        _timer -= 1.0f;
        setTime(_time - 1);

        if (_time == 0)
            gameOver();
    }

    const float dist = abs(_dial->tuneBy(_speed * dt) - _station);
    _screen->setGlitchAmplitude(std::min(1.0f, dist / DETECT_DIST));
    _screen->update(dt);

    if (dist <= LOCK_DIST) {
        _lockTime += dt;

        if (_lockTime >= LOCK_DELAY) {
            setScore(_score + 1);
            _lockTime = 0.0f;
            newStation();
        }
    }
    else {
        _lockTime = 0.0f;
    }
    _dial->lockTo(_lockTime / LOCK_DELAY);
}

void SignalLockScene::initEndLayer(const Size &size)
{
    _endLayer = LayerColor::create(Color4B(0, 0, 0, 128));
    _endLayer->setVisible(false);
    addChild(_endLayer);

    const auto over = Label::createWithTTF("ROUND OVER", "fonts/arial.ttf", 36.0f);
    over->setPosition(size.width / 2.0f, size.height / 2.0f + 48.0f);
    _endLayer->addChild(over);

    _endScoreLabel = Label::createWithTTF("Score: 0", "fonts/arial.ttf", 24.0f);
    _endScoreLabel->setPosition(size.width / 2.0f, size.height / 2.0f);
    _endLayer->addChild(_endScoreLabel);

    const auto restart = Label::createWithTTF("Press R to restart", "fonts/arial.ttf", 18.0f);
    restart->setPosition(size.width / 2.0f, size.height / 2.0f - 48.0f);
    _endLayer->addChild(restart);
}

void SignalLockScene::setScore(const int score)
{
    _score = score;
    _scoreLabel->setString("Score :" + std::to_string(score));
}

void SignalLockScene::setTime(const int time)
{
    _time = time;
    _timeLabel->setString(StringUtils::format("Time: %2d", time));
}

void SignalLockScene::reset()
{
    _isPlaying = false;
    _dial->reset();
    _screen->reset();
    setScore(0);
    _timer = 0.0f;
    setTime(START_TIME);
    _speed = 0.0f;
    _endLayer->setVisible(false);
    scheduleUpdate();
}

void SignalLockScene::onKeyPressed(EventKeyboard::KeyCode key)
{
    switch (key)
    {
    case EventKeyboard::KeyCode::KEY_LEFT_ARROW:
    case EventKeyboard::KeyCode::KEY_A:
        if (_endLayer->isVisible())
            return;

        startPlaying();
        if (_speed == 0.0f)
            _speed = -_velocity;

        break;
    
    case EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
    case EventKeyboard::KeyCode::KEY_D:
        if (_endLayer->isVisible())
            return;

        startPlaying();
        if (_speed == 0.0f)
            _speed = _velocity;
        break;
    
    case EventKeyboard::KeyCode::KEY_R:
        reset();
        break;

    default:
        break;
    }
}

void SignalLockScene::onKeyReleased(const EventKeyboard::KeyCode key)
{
    switch (key)
    {
    case EventKeyboard::KeyCode::KEY_LEFT_ARROW:
    case EventKeyboard::KeyCode::KEY_A:
        if (_speed == -_velocity)
            _speed = 0.0f;
        break;
    
    case EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
    case EventKeyboard::KeyCode::KEY_D:
        if (_speed == _velocity)
            _speed = 0.0f;
        break;

    default:
        break;
    }
}

void SignalLockScene::newStation()
{
    const float value = _dial->getValue();
    do {
        _station = rand_0_1();
    } while(abs(_station - value) < LOCK_SAFE);
}

void SignalLockScene::startPlaying()
{
    if (_isPlaying)
        return;
    
    _isPlaying = true;
    newStation();
}

void SignalLockScene::gameOver()
{
    _isPlaying = false;
    _endScoreLabel->setString(_scoreLabel->getString());
    _endLayer->setVisible(true);
    unscheduleUpdate();
}
