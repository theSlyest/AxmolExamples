#include "RippleTankScene.h"

#include "Director.h"
#include "EventDispatcher.h"
#include "Theme.h"

USING_NS_AX;

// on "init" you need to initialize your instance
bool RippleTankScene::init()
{
    //////////////////////////////
    // 1. super init first
    if (!Scene::init())
        return false;

    const auto& visibleSize = _director->getVisibleSize();
    const Vec2& origin = _director->getVisibleOrigin();

    const auto backgroundLayer = LayerColor::create(Theme::BACKGROUND);
    addChild(backgroundLayer);

    initHud(origin, visibleSize);
    initWater(visibleSize);
    initContacts();
    initStartPrompt(visibleSize);
    initRoundOver();

    initTouchListener();
    initKeyboardListener();
    initGame();

    _game->reset();
    scheduleUpdate();

    return true;
}

void RippleTankScene::initHud(const Vec2& origin, const Size& visibleSize)
{
    _hud = RippleHud::create(visibleSize);
    _hud->setPosition(origin);
    addChild(_hud);
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
void RippleTankScene::initStartPrompt(const Size& visibleSize)
{
    _startPrompt = StartPrompt::create();
    _startPrompt->centerIn(visibleSize);
    addChild(_startPrompt, 1);
}

void RippleTankScene::initRoundOver()
{
    _roundOver = RoundOverLayer::create();
    addChild(_roundOver, 1);
}

void RippleTankScene::initGame()
{
    _game = std::make_unique<TankGame>(ax::Vec2(Water::GRID_SIZE, Water::GRID_SIZE));
    _game->onScoreChanged = [this](const int score) { _hud->setScore(score); };
    _game->onTimerChanged = [this](const int timer) { _hud->setTimer(timer); };
    _game->onStateChanged = [this](const TankGame::State state) { onStateChanged(state); };
}

void RippleTankScene::onStateChanged(const TankGame::State state) const
{
    switch (state) {
        case TankGame::State::START:
            _touchListener->setEnabled(true);
            _roundOver->hide();
            _water->flatten();
            _startPrompt->setVisible(true);
            syncContacts();
            break;

        case TankGame::State::PLAY:
            _startPrompt->setVisible(false);
            break;

        case TankGame::State::END:
            _touchListener->setEnabled(false);
            _water->flatten();
            _roundOver->show(_game->getScore());
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
