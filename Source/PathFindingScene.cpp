#include "PathFindingScene.h"

USING_NS_AX;

// on "init" you need to initialize your instance
bool PathFindingScene::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Scene::init() )
        return false;

    const auto& visibleSize = Director::getInstance()->getVisibleSize();

    initGrid();
    initMode(visibleSize);
    initStep(visibleSize);
    initKeyboardListener();
    initMouseListener();

    return true;
}

void PathFindingScene::initGrid()
{
    _grid = Grid::create();
    addChild(_grid);
}

void PathFindingScene::initMode(const Size& visibleSize)
{
    _isModeSearch = false;
    _modeLabel = Label::createWithTTF("Mode: Edit", "fonts/arial.ttf", 24.0f);
    _modeLabel->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
    _modeLabel->setPosition(20.0f, visibleSize.height - 20.0f);
    addChild(_modeLabel, 1);
}

void PathFindingScene::initStep(const Size& visibleSize)
{
    _stepLabel = Label::createWithTTF("Path: 0", "fonts/arial.ttf", 24.0f);
    _stepLabel->setAnchorPoint(Vec2::ANCHOR_TOP_RIGHT);
    _stepLabel->setPosition(visibleSize.width - 20.0f, visibleSize.height - 20.0f);
    addChild(_stepLabel, 1);
}

void PathFindingScene::setMode(const bool isSearch)
{
    _isModeSearch = isSearch;
    _mouseListener->setEnabled(!isSearch);
    _modeLabel->setString(std::string("Mode: ") + (isSearch ? "Search" : "Edit"));
}

void PathFindingScene::setStep(const int step) const
{
    if (step == 0) {
        _stepLabel->setString(StringUtils::format("Path: ERROR"));
    }
    else {
        _stepLabel->setString(StringUtils::format("Path: %d", step));
    }
}

void PathFindingScene::initKeyboardListener()
{
    _keyListener = EventListenerKeyboard::create();
    _keyListener->onKeyPressed = [this] (const EventKeyboard::KeyCode key, Event *) { onKeyPressed(key); };
    _eventDispatcher->addEventListenerWithSceneGraphPriority(_keyListener, this);
}

void PathFindingScene::onKeyPressed(EventKeyboard::KeyCode key)
{
    if (key == EventKeyboard::KeyCode::KEY_R) {
        reset();
    } else if (key == EventKeyboard::KeyCode::KEY_SPACE && !_isModeSearch) {
        setMode(true);
        const std::vector<Coords>& path = PathFinder::run(_grid->pathFinderGrid(), {START_ROW, START_COL}, {TARGET_ROW, TARGET_COL});
        displayPath(path);
    }
}

void PathFindingScene::reset()
{
    setMode(false);
    _grid->reset();
    _stepLabel->setString(StringUtils::format("Path: 0"));
}

void PathFindingScene::initMouseListener()
{
    _mouseListener = EventListenerMouse::create();
    _mouseListener->onMouseUp = [this](const EventMouse *event) { return onMouseUp(event); };
    _eventDispatcher->addEventListenerWithSceneGraphPriority(_mouseListener, this);
}

bool PathFindingScene::onMouseUp(const ax::EventMouse *event) const
{
    if (event->getMouseButton() == EventMouse::MouseButton::BUTTON_LEFT && !_isModeSearch) {
        _grid->click(event->getLocation());
        return true;
    }

    return false;
}

void PathFindingScene::displayPath(const std::vector<Coords>& path) const
{
    setStep(path.size());
    for (const auto& [x, y] : path) {
        _grid->setPath(x, y);
    }
}
