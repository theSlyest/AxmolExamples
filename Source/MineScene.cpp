#include "MineScene.h"

USING_NS_AX;

Scene* MineScene::createScene()
{
    return MineScene::create();
}

bool MineScene::init()
{
    if ( !Scene::init() )
    {
        return false;
    }

    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    initGrid(visibleSize, origin);
    initMinesLabel(visibleSize, origin);
    initTimerLabel(visibleSize, origin);
    initResetButton(visibleSize, origin);
    initResultLayer(visibleSize, origin);
    initMouseListener();

    _state = MineState::EMPTY;
    setMinesLeft(MINES);
    _timer = 0;
    startTimer();

    return true;
}

void MineScene::initGrid(const Size &size, const Vec2 &origin)
{
    _grid = MineGrid::create();
    _grid->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    _grid->setPosition(origin.x + size.width / 2.0f, origin.y + size.height / 2.0f);
    addChild(_grid);
}

void MineScene::initMinesLabel(const ax::Size &size, const ax::Vec2 &origin)
{
    auto label = Label::createWithTTF("MINES LEFT", "fonts/arial.ttf", 24.0f);
    label->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
    label->setPosition(origin.x + 64.0f, origin.y + size.height - 16.0f);
    addChild(label);

    _minesLbl = Label::createWithTTF("0", "fonts/arial.ttf", 32.0f);
    _minesLbl->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    _minesLbl->setPosition(label->getBoundingBox().getMidX(), origin.y + size.height - 48.0f);
    addChild(_minesLbl);
}

void MineScene::initTimerLabel(const ax::Size &size, const ax::Vec2 &origin)
{
    auto label = Label::createWithTTF("TIMER", "fonts/arial.ttf", 24.0f);
    label->setAnchorPoint(Vec2::ANCHOR_TOP_RIGHT);
    label->setPosition(origin.x + size.width - 64.0f, origin.y + size.height - 16.0f);
    addChild(label);

    _timerLbl = Label::createWithTTF("00:00", "fonts/arial.ttf", 32.0f);
    _timerLbl->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    _timerLbl->setPosition(label->getBoundingBox().getMidX(), origin.y + size.height - 48.0f);
    addChild(_timerLbl);
}

void MineScene::initResetButton(const ax::Size &size, const ax::Vec2 &origin)
{
    const Vec2 pos(origin.x + size.width / 2.0f, origin.y + 48.0f);
    auto rect = DrawNode::create();

    auto label = Label::createWithTTF("RESET", "fonts/arial.ttf", 32.0f);
    auto menuItem = MenuItemLabel::create(label, [this] (Object* sender) { reset(); });
    auto menu = Menu::create(menuItem, nullptr);
    menu->setPosition(pos);

    Size rectSize = label->getContentSize() + Size(32.0f, 24.0f);
    rect->setContentSize(rectSize);
    rect->drawRect(Vec2::ZERO, Vec2(rectSize), Color4F(0.6f, 0.32f, 0.98f, 1.0f), 4.0f);
    rect->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    rect->setPosition(pos);

    addChild(rect);
    addChild(menu);
}

void MineScene::initResultLayer(const ax::Size &size, const ax::Vec2 &origin)
{
    Size layerSize(400.0f, 240.0f);
    _resultLayer = LayerColor::create(Color4B(0, 0, 0, 128), layerSize.width, layerSize.height);
    _resultLayer->setVisible(false);
    _resultLayer->setPosition(origin.x + (size.width - layerSize.width) / 2.0f, origin.y + (size.height - layerSize.height) / 2.0f);
    addChild(_resultLayer);

    _resultLbl = Label::createWithTTF(" ", "fonts/arial.ttf", 32.0f);
    _resultLbl->setPosition(layerSize.width / 2.0f, 3.0f * layerSize.height / 4.0f);
    _resultLayer->addChild(_resultLbl);

    _endTimerLbl = Label::createWithTTF("Time: 00:00", "fonts/arial.ttf", 24.0f);
    _endTimerLbl->setPosition(Vec2(layerSize / 2.0f));
    _resultLayer->addChild(_endTimerLbl);

    auto label = Label::createWithTTF("Press Reset to Play Again", "fonts/arial.ttf", 20.0f);
    label->setPosition(layerSize.width / 2.0f, layerSize.height / 4.0f);
    _resultLayer->addChild(label);
}

void MineScene::initMouseListener()
{
    _mouseListener = EventListenerMouse::create();
    _mouseListener->onMouseDown = [this](EventMouse *event) { return onMouseDown(event); };
    _mouseListener->onMouseMove = [this](EventMouse *event) { return onMouseMove(event); };
    _eventDispatcher->addEventListenerWithSceneGraphPriority(_mouseListener, this);
}

bool MineScene::onMouseDown(ax::EventMouse *event)
{
    if (_state == MineState::FINISHED)
        return false;

    const Vec2& location = event->getLocation();
    EventMouse::MouseButton button = event->getMouseButton();

    if (button == EventMouse::MouseButton::BUTTON_LEFT) {
        if (_state == MineState::EMPTY) {
            if (_grid->populateFrom(location, MINES))
                _state = MineState::ACTIVE;
        }
        if (_grid->reveal(location)) {
            if (_grid->isMineRevealed())
                showResult(false);
            else if (_grid->countCovered() == MINES) {
                showResult(true);
            }
        }
    }
    else if (button == EventMouse::MouseButton::BUTTON_RIGHT) {
        if (_grid->switchFlag(location))
            setMinesLeft(MINES - _grid->countFlags());
    }
    return true;
}

bool MineScene::onMouseMove(ax::EventMouse *event)
{
    if (_state != MineState::FINISHED) {
        _grid->hover(event->getLocation());
        return true;
    }
    return false;
}

void MineScene::setMinesLeft(int value)
{
    _minesLbl->setString(std::to_string(value));
}

void MineScene::startTimer()
{
    schedule([this](float dt) {
        ++_timer;
        _timerLbl->setString(StringUtils::format("%02d:%02d", _timer / 60, _timer % 60));
    }, 1.0f, TIMER_KEY);
}

void MineScene::stopTimer()
{
    unschedule(TIMER_KEY);
}

void MineScene::showResult(bool victory)
{
    stopTimer();
    _state = MineState::FINISHED;
    _resultLbl->setString(victory ? "YOU WIN!" : "GAME OVER!");
    _endTimerLbl->setString(_timerLbl->getString());
    _resultLayer->setVisible(true);
}

void MineScene::reset()
{
    _grid->reset();
    _state = MineState::EMPTY;
    setMinesLeft(MINES);
    _resultLayer->setVisible(false);
    stopTimer();
    _timer = 0;
    _timerLbl->setString("00:00");
    startTimer();
}
