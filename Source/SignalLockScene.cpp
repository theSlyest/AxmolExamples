#include "SignalLockScene.h"

#include "Director.h"
#include "fmt/format.h"

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

    initEndLayer(size);

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
    _game->onScoreChanged = [this](const int score) { _scoreLabel->setString("Score: " + std::to_string(score)); };
    _game->onTimeChanged = [this](const int time) { _timeLabel->setString(fmt::format("Time: {:2d}", time)); };
    _game->onGameOver = [this] { gameOver(); };
}

void SignalLockScene::syncViews(const float dt) const
{
    _dial->setValue(_game->getDialValue());
    _dial->lockTo(_game->getLockProgress());
    _screen->setGlitchAmplitude(_game->getGlitch());
    _screen->update(dt);
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

void SignalLockScene::reset()
{
    _game->reset();
    syncViews(0.0f);
    _screen->reset();
    _endLayer->setVisible(false);
    scheduleUpdate();
}

void SignalLockScene::gameOver()
{
    _endScoreLabel->setString(_scoreLabel->getString());
    _endLayer->setVisible(true);
    unscheduleUpdate();
}
