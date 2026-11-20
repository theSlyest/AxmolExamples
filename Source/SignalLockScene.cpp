#include "SignalLockScene.h"

#include "Director.h"

USING_NS_AX;

// on "init" you need to initialize your instance
bool SignalLockScene::init()
{
    if ( !Scene::init() )
        return false;

    const Vec2& size = _director->getVisibleSize();
    const Vec2& origin = _director->getVisibleOrigin();

    const auto bg = LayerColor::create(Color4B(0x0b, 0x0f, 0x1a, 255));
    addChild(bg);

    // HUD
    _hud = SignalHud::create(size);
    _hud->setPosition(origin);
    addChild(_hud);

    // Dial
    _dial = SignalDial::create();
    _dial->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    _dial->setPosition(origin.x + size.x / 2.0f, origin.y + DIAL_BOTTOM);
    addChild(_dial);

    // Help
    const auto help = Label::createWithTTF("Hold LEFT or RIGHT to tune", "fonts/arial.ttf", 24.0f);
    help->setPosition(origin.x + size.x / 2.0f, origin.y + HELP_BOTTOM);
    addChild(help);

    // Screen
    _screen = SignalScreen::create();
    _screen->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    _screen->setPosition(origin.x + size.x * 0.5f, origin.y + SCREEN_BOTTOM);
    addChild(_screen);

    // Game over layer
    _endLayer = SignalGameOverLayer::create();
    addChild(_endLayer);

    // Controls
    _controls = SignalControls::create();
    _controls->onRestart = [this] { reset(); };
    addChild(_controls);

    initGame();

    reset();

    return true;
}

void SignalLockScene::update(const float dt)
{
    _game->update(dt, _controls->getDirection());
    syncViews(dt);
}

void SignalLockScene::initGame()
{
    _game = std::make_unique<SignalLockGame>();
    _game->onScoreChanged = [this](const int score) { _hud->setScore(score); };
    _game->onTimeChanged = [this](const int time) { _hud->setTime(time); };
    _game->onGameOver = [this] { gameOver(); };
}

void SignalLockScene::syncViews(const float dt) const
{
    _dial->setValue(_game->getDialValue());
    _dial->lockTo(_game->getLockProgress());
    _screen->setGlitchAmplitude(_game->getGlitch());
    _screen->update(dt);
}

void SignalLockScene::reset()
{
    _game->reset();
    syncViews(0.0f);
    _screen->reset();
    _endLayer->hide();
    scheduleUpdate();
}

void SignalLockScene::gameOver()
{
    _endLayer->show(_game->getScore());
    unscheduleUpdate();
}
