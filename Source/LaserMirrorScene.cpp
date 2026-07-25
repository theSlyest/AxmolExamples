#include "LaserMirrorScene.h"

USING_NS_AX;

Scene* LaserMirrorScene::createScene()
{
    return LaserMirrorScene::create();
}

// on "init" you need to initialize your instance
bool LaserMirrorScene::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Scene::init() )
    {
        return false;
    }

    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

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

void LaserMirrorScene::onExit()
{
    Scene::onExit();
    _eventDispatcher->removeEventListener(_touchListener);
}

void LaserMirrorScene::reset()
{
    _grid->reset(INITIAL_GRID);
    _grid->positionEmitter(EMITTER_SIDE, EMITTER_POS);
    _grid->refreshBeam();

    setMirrors(0);
    setRemaining(INITIAL_MIRRORS);

    _panel->setVisible(false);
    _isSolved = false;
}

void LaserMirrorScene::initGrid(const Size &visibleSize, const Vec2 origin)
{
    _grid = LaserGrid::create();
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
    _hudBest->setPosition(origin.x + visibleSize.width - 64.0f, origin.y + visibleSize.height - HUD_OFFSET);
    addChild(_hudBest);
}

void LaserMirrorScene::initMirrors(const ax::Size &visibleSize, const ax::Vec2 origin)
{
    _hudMirrors = HudValue::create("MIRRORS", Color4F(0.21f, 0.6f, 0.86f, 1.0f));
    _hudMirrors->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
    _hudMirrors->setPosition(origin.x + 64.0f, origin.y + visibleSize.height - HUD_OFFSET);
    addChild(_hudMirrors);
}

void LaserMirrorScene::initRemaining(const ax::Size &visibleSize, const ax::Vec2 origin)
{
    _hudRemaining = HudValue::create("REMAINING", Color4F(0.36f, 0.67f, 0.31f, 1.0f));
    _hudRemaining->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    _hudRemaining->setPosition(origin.x + visibleSize.width / 2.0f, origin.y + visibleSize.height - HUD_OFFSET);
    addChild(_hudRemaining);
}

void LaserMirrorScene::initTouchListener()
{
    _touchListener = EventListenerTouchOneByOne::create();
    _touchListener->onTouchBegan = [this](Touch* t, Event* e) -> bool { return onTouchBegan(t, e); };
    _eventDispatcher->addEventListenerWithSceneGraphPriority(_touchListener, this);
}

bool LaserMirrorScene::onTouchBegan(Touch *touch, Event *event)
{
    if (!_isSolved && _grid->onTouch(touch, _grid->countMirrors() == INITIAL_MIRRORS)) {
        int mirrorCount = _grid->countMirrors();
        setMirrors(mirrorCount);
        setRemaining(INITIAL_MIRRORS - mirrorCount);

        if (_grid->isTargetReached()) {
            _isSolved = true;
            showPanel();
        }

        return true;
    }

    return false;
}

void LaserMirrorScene::setBest(int value) 
{
    _best = value;
    _hudBest->setValue(std::to_string(value)); 
}

void LaserMirrorScene::setMirrors(int value) 
{
    _mirrorCount = value; 
    _hudMirrors->setValue(std::to_string(value)); 
}

void LaserMirrorScene::setRemaining(int value) 
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

void LaserMirrorScene::saveBestValue()
{
    UserDefault::getInstance()->setIntegerForKey(BEST_KEY, _best);
    UserDefault::getInstance()->flush();
}
