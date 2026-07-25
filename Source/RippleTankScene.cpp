#include "RippleTankScene.h"

USING_NS_AX;

const Color4B RippleTankScene::TextColor(42, 169, 192, 255); // #2aa9c0

// on "init" you need to initialize your instance
bool RippleTankScene::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Scene::init() )
    {
        return false;
    }

    const auto& visibleSize = Director::getInstance()->getVisibleSize();
    const Vec2& origin = Director::getInstance()->getVisibleOrigin();

    const auto backgroundLayer = LayerColor::create(Color4B(10, 20, 34, 255)); // #0a1422
    addChild(backgroundLayer);

    initScoreLabel(origin, visibleSize);
    initTimerLabel(origin, visibleSize);
    initWater(visibleSize);
    initContacts();
    initStartLayer(visibleSize);
    initEndLayer(visibleSize);

    initTouchListener();
    initKeyboardListener();

    setStateStart();

    return true;
}

void RippleTankScene::initScoreLabel(const Vec2& origin, const Size& visibleSize)
{
    _scoreLabel = Label::createWithTTF("Score: 0", "fonts/arial.ttf", 24.0f);
    _scoreLabel->setTextColor(TextColor);
    _scoreLabel->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
    _scoreLabel->setPosition(origin.x + 16.0f, origin.y + visibleSize.height - 16.0f);
    addChild(_scoreLabel);
}

void RippleTankScene::initTimerLabel(const Vec2& origin, const Size& visibleSize)
{
    _timerLabel = Label::createWithTTF("Timer: 45", "fonts/arial.ttf", 24.0f);
    _timerLabel->setTextColor(TextColor);
    _timerLabel->setAnchorPoint(Vec2::ANCHOR_TOP_RIGHT);
    _timerLabel->setPosition(origin.x + visibleSize.width - 16.0f, origin.y + visibleSize.height - 16.0f);
    addChild(_timerLabel);
}

void RippleTankScene::setScore(int score)
{
    _score = score;
    _scoreLabel->setString(StringUtils::format("Score: %d", score));
}

void RippleTankScene::setTimer(int timer)
{
    _timer = timer;
    _timerLabel->setString(StringUtils::format("Timer: %d", timer));
}

void RippleTankScene::initWater(const Size& visibleSize)
{
    _water = Water::create();
    _water->setPosition(visibleSize.width / 2.0f, visibleSize.height / 2.0f - 20.0f);
    addChild(_water);
    _waterSize = _water->getContentSize();
}

void RippleTankScene::initTouchListener()
{
    _touchListener = EventListenerTouchOneByOne::create();
    _touchListener->onTouchBegan = [this](Touch* touch, Event* event) -> bool { 
        return onTouchBegan(touch, event); 
    };
    _eventDispatcher->addEventListenerWithSceneGraphPriority(_touchListener, _water);
}

bool RippleTankScene::onTouchBegan(ax::Touch* touch, ax::Event* event)
{
    if (!_isStatePlay) {
        setStatePlay();
    }

    if (_water->onTouch(touch, event)) {
        Vec2 waterLocation = _water->convertTouchToNodeSpace(touch);
        checkContactReached(waterLocation);
        return true;
    }

    return false;
}

void RippleTankScene::initKeyboardListener()
{
    _keyboardListener = EventListenerKeyboard::create();
    _keyboardListener->onKeyPressed = [this](EventKeyboard::KeyCode key, Event* event) {
        onKeyPressed(key, event);
    };
    _eventDispatcher->addEventListenerWithSceneGraphPriority(_keyboardListener, this);
}

void RippleTankScene::onKeyPressed(EventKeyboard::KeyCode key, Event* event)
{
    if (key ==  EventKeyboard::KeyCode::KEY_R)
        setStateStart();
}

void RippleTankScene::initContacts()
{
    for (int i = 0; i < 4; ++i) {
        auto contact = Contact::create();
        _water->addChild(contact);
        _contacts.push_back(contact);
    }
}
void RippleTankScene::initStartLayer(const Size& visibleSize)
{
    auto label = Label::createWithTTF("Click the surface to send a ripple", "fonts/arial.ttf", 24.0f);
    label->setTextColor(TextColor);
    Size layerSize = label->getContentSize() + Size(20.f, 20.f);

    _startLayer = LayerColor::create(Color4B(10, 20, 34, 255));
    _startLayer->setContentSize(layerSize);

    label->setPosition(layerSize / 2.0f);
    _startLayer->addChild(label);
    _startLayer->setPosition((visibleSize - layerSize) / 2.0f);
    addChild(_startLayer, 1);
}

void RippleTankScene::initEndLayer(const Size& visibleSize)
{
    _endLayer = LayerColor::create(Color4B(10, 20, 34, 192));
    _endLayer->setVisible(false);
    addChild(_endLayer, 1);

    float midX = visibleSize.width / 2.0f;
    float midY = visibleSize.height / 2.0f;

    auto endLabel = Label::createWithTTF("ROUND OVER", "fonts/arial.ttf", 24.0f);
    endLabel->setTextColor(TextColor);
    endLabel->setPosition(midX, midY + 28.0f);
    _endLayer->addChild(endLabel);

    _endScoreLabel = Label::createWithTTF("Score: 0", "fonts/arial.ttf", 24.0f);
    _endScoreLabel->setTextColor(TextColor);
    _endScoreLabel->setPosition(midX, midY);
    _endLayer->addChild(_endScoreLabel);

    auto startLabel = Label::createWithTTF("Press R to restart", "fonts/arial.ttf", 24.0f);
    startLabel->setTextColor(TextColor);
    startLabel->setPosition(midX, midY - 28.0f);
    _endLayer->addChild(startLabel);
}

void RippleTankScene::setStateStart()
{
    _isStatePlay = false;

    unscheduleUpdate();
    unschedule("updateTimer");

    _touchListener->setEnabled(true);
    _endLayer->setVisible(false);
    _water->flatten();

    setScore(0);
    setTimer(45);

    for (Contact* contact : _contacts) {
        contact->setPosition(randomWaterLocation());
        contact->setRandomDirection();
    }

    _startLayer->setVisible(true);
}

void RippleTankScene::setStatePlay()
{
    _isStatePlay = true;
    scheduleUpdate();
    updateTimer();
    schedule([this](float dt) { updateTimer(); }, 1.0f, "updateTimer");
    _startLayer->setVisible(false);
}

void RippleTankScene::setStateEnd()
{
    _isStatePlay = false;
    unscheduleUpdate();
    unschedule("updateTimer");

    _touchListener->setEnabled(false);
    _water->flatten();

    _endScoreLabel->setString(_scoreLabel->getString());
    _endLayer->setVisible(true);
}

Vec2 RippleTankScene::randomWaterLocation()
{
    const float padding = 16.0f;
    return Vec2(
        random(padding, _waterSize.width - padding),
        random(padding, _waterSize.height - padding));
}

bool RippleTankScene::isWithinReach(Vec2 posA, Vec2 posB)
{
    return posA.getDistance(posB) <= RIPPLE_REACH;
}

void RippleTankScene::respawnContact(Contact* contact, ax::Vec2 touchLocation)
{
    Vec2 newLoc;
    do {
        newLoc = randomWaterLocation();
    } while (isWithinReach(touchLocation, newLoc));
    contact->setPosition(newLoc);
    contact->setRandomDirection();
}

void RippleTankScene::checkContactReached(Vec2 touchLocation)
{
    for (Contact* contact : _contacts) {
        if (isWithinReach(touchLocation, contact->getPosition())) {
            setScore(_score + 1);
            respawnContact(contact, touchLocation);
        }
    }
}

void RippleTankScene::updateContact(float dt, Contact* contact)
{
    Vec2 pos = contact->getPosition();
    pos += contact->getDirection() * dt;
    contact->setPosition(pos);

    Vec2 dir = contact->getDirection();

    // Water vertical limits
    if (pos.x - CON_RADIUS <= 0.0f || pos.x + CON_RADIUS >= _waterSize.width) {
        dir.x *= -1;
        contact->setDirection(dir);
    }

    // Water horizontal limits
    if (pos.y - CON_RADIUS <= 0.0f || pos.y + CON_RADIUS >= _waterSize.height) {
        dir.y *= -1;
        contact->setDirection(dir);
    }
}

void RippleTankScene::updateTimer()
{
    if (const int timer = _timer - 1; timer == -1) {
        setStateEnd();
    }
    else {
        setTimer(timer);
    }
}

void RippleTankScene::update(float dt)
{
    for (Contact* contact : _contacts) {
        updateContact(dt, contact);
    }
}
