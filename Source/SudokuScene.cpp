#include "SudokuScene.h"

USING_NS_AX;

Scene* SudokuScene::createScene()
{
    return SudokuScene::create();
}

// on "init" you need to initialize your instance
bool SudokuScene::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Scene::init() )
    {
        return false;
    }

    auto bg = LayerColor::create(Color4B(23, 27, 34, 255));
    addChild(bg);

    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    initTitle(visibleSize, origin);
    initStatus(visibleSize, origin);
    initHelp(visibleSize, origin);
    initGrid(visibleSize, origin);
    initSolved();

    initMouseListener();
    initKeyboardListener();

    reset();

    return true;
}

void SudokuScene::initTitle(const Size &visibleSize, const Vec2 &origin)
{
    auto title = Label::createWithTTF("SUDOKU", "fonts/arial.ttf", 36.0f);
    title->setTextColor(Color4B(224, 165, 63, 255));
    title->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
    title->setPosition(Vec2(origin.x + 20.0f, origin.y + visibleSize.height - 20.0f));
    addChild(title);
}

void SudokuScene::initGrid(const Size& visibleSize, const Vec2& origin)
{
    _grid = SudokuGrid::create();
    _grid->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    _grid->setPosition(origin.x + visibleSize.width / 2.0f, origin.y + visibleSize.height / 2.0f);
    addChild(_grid);
}

void SudokuScene::initStatus(const Size &visibleSize, const Vec2 &origin)
{
    _status = Label::createWithTTF("Cells left: 0", "fonts/arial.ttf", 24.0f);
    _status->setTextColor(StatusColor);
    _status->setPosition(origin.x + visibleSize.width / 2.0f, origin.y + 64.0f);
    addChild(_status);
}

void SudokuScene::initHelp(const ax::Size &visibleSize, const ax::Vec2 &origin)
{
    Label* help = Label::createWithTTF("Click a cell, type 1-9, 0 or Backspace clears, R to restart", "fonts/arial.ttf", 18.0f);
    help->setTextColor(StatusColor);
    help->setPosition(origin.x + visibleSize.width / 2.0f, origin.y + 32.0f);
    addChild(help);
}

void SudokuScene::initSolved()
{
    _solved = LayerColor::create(Color4B(16, 19, 26, 255), _grid->getContentSize().width, _grid->getContentSize().height);
    _solved->setPosition(_grid->getBoundingBox().origin);
    _solved->setVisible(false);
    addChild(_solved, 1);

    auto label = Label::createWithTTF("SOLVED", "fonts/arial.ttf", 48.0f);
    label->setTextColor(Color4B(230, 230, 238, 255));
    label->setPosition(Vec2(_grid->getContentSize()) / 2.0f);
    _solved->addChild(label);

    auto restart = Label::createWithTTF("Press R to restart", "fonts/arial.ttf", 18.0f);
    restart->setTextColor(Color4B(230, 230, 238, 255));
    restart->setPosition(_grid->getContentSize().width / 2.0f, _grid->getContentSize().height / 2.0f - 56.0f);
    _solved->addChild(restart);
}

void SudokuScene::initMouseListener()
{
    _mouseListener = EventListenerMouse::create();
    _mouseListener->onMouseDown = [this](EventMouse *event) { return onMouseDown(event); };
    _eventDispatcher->addEventListenerWithSceneGraphPriority(_mouseListener, this);
}

bool SudokuScene::onMouseDown(ax::EventMouse *event)
{
    GridCell* cell = _grid->click(event->getLocation());
    if (cell == nullptr)
        return false;

    return true;
}

void SudokuScene::initKeyboardListener()
{
    _keyboardListener = EventListenerKeyboard::create();
    _keyboardListener->onKeyPressed = [this](EventKeyboard::KeyCode key, ax::Event *) { onKeyPressed(key); };
    _eventDispatcher->addEventListenerWithSceneGraphPriority(_keyboardListener, this);
}

void SudokuScene::onKeyPressed(EventKeyboard::KeyCode key)
{
    if (key >= EventKeyboard::KeyCode::KEY_1 && key <= EventKeyboard::KeyCode::KEY_9) {
        const char digit = '1' + ((char)key - (char)EventKeyboard::KeyCode::KEY_1);
        if (_grid->setDigit(digit)) {
            _grid->checkConflict();
        }
        int left = _grid->countLeft();
        setStatus(left);
        if (left == 0) {
            if (!_grid->checkAll()) {
                showSolved();
            }
        }
    }
    else if (key == EventKeyboard::KeyCode::KEY_0 || key == EventKeyboard::KeyCode::KEY_BACKSPACE  || key == EventKeyboard::KeyCode::KEY_DELETE) {
        if (_grid->erase())
            setStatus(_grid->countLeft());
    }
    else if (key == EventKeyboard::KeyCode::KEY_R) {
        reset();
    }
}

void SudokuScene::setStatus(const int leftCount)
{
    _status->setString("Cells left: " + std::to_string(leftCount));
}

void SudokuScene::reset()
{
    _grid->clear();
    _grid->loadClues(CLUES);
    setStatus(_grid->countLeft());
    _mouseListener->setEnabled(true);
    _solved->setVisible(false);
}

void SudokuScene::showSolved()
{
    _grid->deselect();
    _mouseListener->setEnabled(false);
    _solved->setVisible(true);
}
