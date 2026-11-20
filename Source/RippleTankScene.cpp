#include "RippleTankScene.h"

#include "Director.h"
#include "EventDispatcher.h"
#include "fmt/format.h"

USING_NS_AX;

const Color4B RippleTankScene::TextColor(42, 169, 192, 255); // #2aa9c0

// on "init" you need to initialize your instance
bool RippleTankScene::init()
{
    //////////////////////////////
    // 1. super init first
    if (!Scene::init())
        return false;

    const auto& visibleSize = _director->getVisibleSize();
    const Vec2& origin = _director->getVisibleOrigin();

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
    initGame();

    _game->reset();
    scheduleUpdate();

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

void RippleTankScene::initWater(const Size& visibleSize)
{
    _water = Water::create();
    _water->setPosition(visibleSize.width / 2.0f, visibleSize.height / 2.0f - 20.0f);
    addChild(_water);
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
    _game->start();

    if (_water->onTouch(touch, event)) {
        _game->strike(_water->convertTouchToNodeSpace(touch));
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

void RippleTankScene::onKeyPressed(const EventKeyboard::KeyCode key, Event* event) const
{
    if (key == EventKeyboard::KeyCode::KEY_R)
        _game->reset();
}

void RippleTankScene::initContacts()
{
    for (int i = 0; i < TankGame::CONTACT_COUNT; ++i) {
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

    const float midX = visibleSize.width / 2.0f;
    const float midY = visibleSize.height / 2.0f;

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

void RippleTankScene::initGame()
{
    _game = std::make_unique<TankGame>(ax::Vec2(Water::GRID_SIZE, Water::GRID_SIZE));
    _game->onScoreChanged = [this](const int score) { _scoreLabel->setString(fmt::format("Score: {}", score)); };
    _game->onTimerChanged = [this](const int timer) { _timerLabel->setString(fmt::format("Timer: {}", timer)); };
    _game->onStateChanged = [this](const TankGame::State state) { onStateChanged(state); };
}

void RippleTankScene::onStateChanged(const TankGame::State state) const
{
    switch (state) {
        case TankGame::State::START:
            _touchListener->setEnabled(true);
            _endLayer->setVisible(false);
            _water->flatten();
            _startLayer->setVisible(true);
            syncContacts();
            break;

        case TankGame::State::PLAY:
            _startLayer->setVisible(false);
            break;

        case TankGame::State::END:
            _touchListener->setEnabled(false);
            _water->flatten();
            _endScoreLabel->setString(_scoreLabel->getString());
            _endLayer->setVisible(true);
            break;
    }
}

void RippleTankScene::syncContacts() const
{
    const auto& contacts = _game->getContacts();
    for (size_t i = 0; i < _contacts.size(); ++i)
        _contacts[i]->setPosition(contacts[i].position);
}

void RippleTankScene::update(const float dt)
{
    _game->update(dt);
    syncContacts();
}
