#include "LaserMirrorScene.h"

#include "Director.h"
#include "EventDispatcher.h"
#include "UserDefault.h"

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
    initBest(visibleSize, origin);
    initMirrors(visibleSize, origin);
    initRemaining(visibleSize, origin);
    initTouchListener();

    loadBestValue();

    reset();

    return true;
}

void LaserMirrorScene::reset()
{
    _board->reset();
    _grid->refresh();

    setMirrors(0);
    setRemaining(INITIAL_MIRRORS);

    _panel->setVisible(false);
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
    _panel->setButtonCallback([this](Object* sender) { reset(); });
    _panel->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    _panel->setPosition(origin.x + visibleSize.width / 2.0f, origin.y + visibleSize.height / 2.0f);
    _panel->setVisible(false);
    addChild(_panel);
}

void LaserMirrorScene::initBest(const ax::Size &visibleSize, const ax::Vec2 origin)
{
    _hudBest = HudValue::create("BEST", Color4F(0.95f, 0.77f, 0.06f, 1.0f));
    _hudBest->setAnchorPoint(Vec2::ANCHOR_TOP_RIGHT);
    _hudBest->setPosition(origin.x + visibleSize.width - HUD_MARGIN, origin.y + visibleSize.height - HUD_TOP);
    addChild(_hudBest);
}

void LaserMirrorScene::initMirrors(const ax::Size &visibleSize, const ax::Vec2 origin)
{
    _hudMirrors = HudValue::create("MIRRORS", Color4F(0.21f, 0.6f, 0.86f, 1.0f));
    _hudMirrors->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
    _hudMirrors->setPosition(origin.x + HUD_MARGIN, origin.y + visibleSize.height - HUD_TOP);
    addChild(_hudMirrors);
}

void LaserMirrorScene::initRemaining(const ax::Size &visibleSize, const ax::Vec2 origin)
{
    _hudRemaining = HudValue::create("REMAINING", Color4F(0.36f, 0.67f, 0.31f, 1.0f));
    _hudRemaining->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    _hudRemaining->setPosition(origin.x + visibleSize.width / 2.0f, origin.y + visibleSize.height - HUD_TOP);
    addChild(_hudRemaining);
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

    const int mirrorCount = _board->getMirrorCount();
    setMirrors(mirrorCount);
    setRemaining(_board->getRemainingMirrors());

    if (_board->isSolved()) {
        _isSolved = true;
        showPanel();
    }

    return true;
}

void LaserMirrorScene::setBest(const int value)
{
    _best = value;
    _hudBest->setValue(std::to_string(value)); 
}

void LaserMirrorScene::setMirrors(const int value)
{
    _mirrorCount = value; 
    _hudMirrors->setValue(std::to_string(value)); 
}

void LaserMirrorScene::setRemaining(const int value) const
{ 
    _hudRemaining->setValue(std::to_string(value)); 
}

void LaserMirrorScene::showPanel()
{
    _isSolved = true;
    std::string bestResult;
    if (_mirrorCount < _best) {
        setBest(_mirrorCount);
        saveBestValue();
        bestResult = std::to_string(_best) + " (New record)";
    }
    else {
        bestResult = std::to_string(_best);
    }
    _panel->setValues(std::to_string(_mirrorCount), bestResult);
    _panel->setVisible(true);
}

void LaserMirrorScene::loadBestValue()
{
    setBest(UserDefault::getInstance()->getIntegerForKey(BEST_KEY, INITIAL_MIRRORS));
}

void LaserMirrorScene::saveBestValue() const
{
    UserDefault::getInstance()->setIntegerForKey(BEST_KEY, _best);
    UserDefault::getInstance()->flush();
}
