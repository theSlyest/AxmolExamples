#include "RippleTankScene.h"

#include "Director.h"
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

    initControls();
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

void RippleTankScene::initControls()
{
    _controls = RippleControls::create();
    _controls->onTouch = [this](Touch* touch) { return onTouch(touch); };
    _controls->onRestart = [this] { _game->reset(); };
    addChild(_controls);
}

bool RippleTankScene::onTouch(Touch* touch) const
{
    _game->start();

    if (const auto position = _water->locate(touch)) {
        _water->sendRipple(*position);
        _game->strike(*position);
        return true;
    }

    return false;
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
            _controls->setTouchEnabled(true);
            _roundOver->hide();
            _water->flatten();
            _startPrompt->setVisible(true);
            syncContacts();
            break;

        case TankGame::State::PLAY:
            _startPrompt->setVisible(false);
            break;

        case TankGame::State::END:
            _controls->setTouchEnabled(false);
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
