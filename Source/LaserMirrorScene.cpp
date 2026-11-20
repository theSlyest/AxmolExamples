#include "LaserMirrorScene.h"

#include "Director.h"
#include "EventDispatcher.h"

USING_NS_AX;

// on "init" you need to initialize your instance
bool LaserMirrorScene::init()
{
    //////////////////////////////
    // 1. super init first
    if (!Scene::init())
        return false;

    const auto visibleSize = _director->getVisibleSize();
    const Vec2 origin = _director->getVisibleOrigin();

    _board = std::make_unique<LaserBoard>(LEVEL, EMITTER_SIDE, EMITTER_POS, INITIAL_MIRRORS);

    initGrid(visibleSize, origin);
    initPanel(visibleSize, origin);
    initHud(visibleSize, origin);
    initTouchListener();

    reset();

    return true;
}

void LaserMirrorScene::reset()
{
    _board->reset();
    _grid->refresh();

    updateHud();
    _hud->setBest(_best.get());

    _panel->hide();
    _isSolved = false;
}

void LaserMirrorScene::initGrid(const Size &visibleSize, const Vec2 origin)
{
    _grid = LaserGrid::create(*_board);
    _grid->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    _grid->setPosition(origin.x + visibleSize.width / 2.0f, origin.y + visibleSize.height / 2.0f);
    addChild(_grid);
}

void LaserMirrorScene::initPanel(const ax::Size &visibleSize, const ax::Vec2 origin)
{
    _panel = SolvedPanel::create();
    _panel->setButtonCallback([this](Object*) { reset(); });
    _panel->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    _panel->setPosition(origin.x + visibleSize.width / 2.0f, origin.y + visibleSize.height / 2.0f);
    _panel->hide();
    addChild(_panel);
}

void LaserMirrorScene::initHud(const Size &visibleSize, const Vec2 origin)
{
    _hud = LaserHud::create(visibleSize);
    _hud->setPosition(origin);
    addChild(_hud);
}

void LaserMirrorScene::initTouchListener()
{
    // The listener is attached to the scene, so it goes away with it.
    const auto listener = EventListenerTouchOneByOne::create();
    listener->onTouchBegan = [this](Touch* t, Event* e) -> bool { return onTouchBegan(t, e); };
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
}

bool LaserMirrorScene::onTouchBegan(Touch *touch, Event *event)
{
    if (_isSolved)
        return false;

    const auto cell = _grid->cellAt(touch);
    if (!cell)
        return false;

    _board->cycleMirror(cell->first, cell->second);
    _grid->refresh();

    updateHud();

    if (_board->isSolved()) {
        _isSolved = true;
        showPanel();
    }

    return true;
}

void LaserMirrorScene::updateHud() const
{
    _hud->setMirrors(_board->getMirrorCount());
    _hud->setRemaining(_board->getRemainingMirrors());
}

void LaserMirrorScene::showPanel()
{
    const bool newRecord = _best.submit(_board->getMirrorCount());
    if (newRecord)
        _hud->setBest(_best.get());

    _panel->show(_board->getMirrorCount(), _best.get(), newRecord);
}
