#include "RushHourScene.h"

USING_NS_AX;

const std::vector<Color4F> RushHourScene::V_COLORS {
    Color4F(0.87f, 0.28f, 0.23f, 1.0f), // red
    Color4F(0.32f, 0.71f, 0.13f, 1.0f), // green
    Color4F(0.43f, 0.02f, 0.92f, 1.0f), // purple
    Color4F(0.92f, 0.76f, 0.00f, 1.0f), // yellow
    Color4F(0.26f, 0.64f, 0.61f, 1.0f), // cyan
    Color4F(0.92f, 0.41f, 0.00f, 1.0f), // orange
    Color4F(0.28f, 0.36f, 0.91f, 1.0f), // blue
};

const std::vector<std::pair<int, int>> RushHourScene::V_POS { { 0, 3}, { 4, 5}, { 4, 0}, { 4, 2}, { 5, 1}, { 0, 0}, { 2, 3}, };

Scene* RushHourScene::createScene()
{
    return RushHourScene::create();
}

bool RushHourScene::init()
{
    if ( !Scene::init() )
    {
        return false;
    }

    const auto& size = Director::getInstance()->getVisibleSize();
    const Vec2& origin = Director::getInstance()->getVisibleOrigin();

    const auto bg = LayerColor::create(Color4B(0x14, 0x18, 0x1f, 0xff));
    addChild(bg);

    const auto title = Label::createWithTTF("RUSH HOUR", "fonts/arial.ttf", 36.0f);
    title->setTextColor(Color4B(0xff, 0xc2, 0x4a, 0xff));
    title->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
    title->setPosition(Vec2(origin.x + UI_OFFSET, origin.y + size.height - UI_OFFSET));
    addChild(title);

    const auto help = Label::createWithTTF("Drag a car along its lane to slide it. Get the red car out the right exit. R to restart", "fonts/arial.ttf", 18.0f);
    help->setPosition(origin.x + size.width / 2.0f, origin.y + UI_OFFSET);
    addChild(help);

    initMoves(origin, size);
    initGrid(origin, size);
    initEndLayer(origin, size);
    initTouchListener();
    initKeyboardListener();

    reset();

    return true;
}

void RushHourScene::initMoves(const Vec2 &origin, const Size &size)
{
    _movesLbl = Label::createWithTTF("Moves: 0", "fonts/arial.ttf", 24.0f);
    _movesLbl->setAnchorPoint(Vec2::ANCHOR_BOTTOM_RIGHT);
    _movesLbl->setPosition(origin.x + size.width - UI_OFFSET, origin.y + size.height - UI_OFFSET);
    addChild(_movesLbl);
}

void RushHourScene::initGrid(const Vec2 &origin, const Size& size)
{
    _grid = RushHourGrid::create();
    _grid->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    _grid->setPosition(origin.x + size.width / 2.0f, origin.y + size.height / 2.0f);
    addChild(_grid);
    initVehicles();
}

void RushHourScene::initEndLayer(const Vec2 &origin, const Size& size)
{
    constexpr float layerSize = 420.0f;
    _endLayer = LayerColor::create(Color4B(0x33, 0x33, 0x33, 0xff), layerSize, layerSize);
    _endLayer->setVisible(false);
    _endLayer->setPosition(origin.x + (size.width - layerSize) / 2.0f, origin.y + (size.height - layerSize) / 2.0f);
    addChild(_endLayer);

    const auto solved = Label::createWithTTF("SOLVED", "fonts/arial.ttf", 36.0f);
    solved->setPosition(layerSize / 2.0f, layerSize / 2.0f + 40.0f);
    _endLayer->addChild(solved);

    _endMovesLbl = Label::createWithTTF("Moves: 0", "fonts/arial.ttf", 24.0f);
    _endMovesLbl->setPosition(layerSize / 2.0f, layerSize / 2.0f);
    _endLayer->addChild(_endMovesLbl);

    auto restart = Label::createWithTTF("Press R to restart", "fonts/arial.ttf", 18.0f);
    restart->setPosition(layerSize / 2.0f, layerSize / 2.0f - 40.0f);
    _endLayer->addChild(restart);
}

void RushHourScene::initVehicles()
{
    const int limit = V_POS.size();
    for (int i = 0; i < limit; ++i) {
        const bool vertical = (i > 2);
        const bool truck = (i > 4);
        auto vehicle = RushHourVehicle::create(V_COLORS[i], truck, vertical);
        _grid->addChild(vehicle);
        _vehicles.push_back(vehicle);
    }
}

void RushHourScene::initTouchListener()
{
    _touchListener = EventListenerTouchOneByOne::create();
    _touchListener->onTouchBegan = [this](Touch* touch, Event* event) { return onTouchBegan(touch, event); };
    _touchListener->onTouchMoved = [this](Touch* touch, Event* event) { onTouchMoved(touch, event); };
    _touchListener->onTouchEnded = [this](Touch* touch, Event* event) { onTouchEnded(touch, event); };
    _eventDispatcher->addEventListenerWithSceneGraphPriority(_touchListener, this);
}

void RushHourScene::initKeyboardListener()
{
    _keyboardListener = EventListenerKeyboard::create();
    _keyboardListener->onKeyPressed = [this](EventKeyboard::KeyCode key, Event *) { onKeyPressed(key); };
    _eventDispatcher->addEventListenerWithSceneGraphPriority(_keyboardListener, this);
}

void RushHourScene::setMoves(int moves)
{
    _moves = moves;
    _movesLbl->setString("Moves: " + std::to_string(moves));
}

void RushHourScene::resetGrid()
{
    _grid->reset();
    const int limit = V_POS.size();
    for (int i = 0; i < limit; ++i) {
        positionVehicle(_vehicles[i], V_POS[i]);
    }
}

void RushHourScene::reset()
{
    _dragged = nullptr;
    resetGrid();
    setMoves(0);
    _endLayer->setVisible(false);
    _touchListener->setEnabled(true);
}

void RushHourScene::positionVehicle(RushHourVehicle *vehicle, const GridCell &cell) const
{
    vehicle->setPosition(RushHourGrid::getCellPosition(cell));
    const int size = vehicle->getSize();
    const bool vertical = vehicle->isVertical();
    for (int i = 0; i < size; i++) {
        if (vertical)
            _grid->setOccupied(cell.first, cell.second + i, true);
        else
            _grid->setOccupied(cell.first + i, cell.second, true);
    }
}

bool RushHourScene::onTouchBegan(Touch *touch, Event *event)
{
    const Vec2 touchPos = _grid->convertTouchToNodeSpace(touch);
    for (RushHourVehicle* vehicle : _vehicles) {
        if (vehicle->getBoundingBox().containsPoint(touchPos)) {
            _dragged = vehicle;
            _dragPos = touchPos;
            _dragCell = RushHourGrid::getCell(vehicle->getPosition());
            const int size = vehicle->getSize();
            const bool vertical = vehicle->isVertical();
            _grid->freeCells(_dragCell, size, vertical);
            _dragPosMin = _grid->getMinPos(_dragCell.first, _dragCell.second, vertical);
            _dragPosMax = _grid->getMaxPos(_dragCell.first, _dragCell.second, vertical, size);
            return true;
        }
    }
    
    return false;
}

void RushHourScene::onTouchMoved(Touch *touch, Event *event)
{
    if (_dragged == nullptr)
        return;

    const Vec2 touchPos = _grid->convertTouchToNodeSpace(touch);
    const Vec2 delta = touchPos - _dragPos;
    Vec2 newPos = _dragged->isVertical() 
        ? Vec2(_dragged->getPositionX(), _dragged->getPositionY() + delta.y)
        : Vec2(_dragged->getPositionX() + delta.x, _dragged->getPositionY());

    if (newPos < _dragPosMin)
        _dragged->setPosition(_dragPosMin);
    else if (newPos > _dragPosMax)
        _dragged->setPosition(_dragPosMax);
    else 
        _dragged->setPosition(newPos);

    _dragPos = touchPos;
}

void RushHourScene::onTouchEnded(Touch *touch, Event *event)
{
    if (_dragged == nullptr)
        return;

    const GridCell& cell = RushHourGrid::getCell(_dragged->getPosition());
    positionVehicle(_dragged, cell);
    if (cell != _dragCell)
        setMoves(_moves + 1);

    _dragged = nullptr;
    if (cell == EXIT)
        showEndLayer();
}

void RushHourScene::onKeyPressed(const EventKeyboard::KeyCode key)
{
    if (key == EventKeyboard::KeyCode::KEY_R)
        reset();
}

void RushHourScene::showEndLayer() const
{
    _touchListener->setEnabled(false);
    _endMovesLbl->setString(_movesLbl->getString());
    _endLayer->setVisible(true);
}
