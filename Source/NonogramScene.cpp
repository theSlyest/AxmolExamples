#include "NonogramScene.h"

USING_NS_AX;

Scene* NonogramScene::createScene()
{
    return NonogramScene::create();
}

// on "init" you need to initialize your instance
bool NonogramScene::init()
{
    if ( !Scene::init() )
    {
        return false;
    }

    const auto& visibleSize = Director::getInstance()->getVisibleSize();
    const Vec2& origin = Director::getInstance()->getVisibleOrigin();

    const auto bg = LayerColor::create(Color4B(26, 29, 36, 255));
    addChild(bg);

    const auto title = Label::createWithTTF("NONOGRAM", "fonts/arial.ttf", 20.0f);
    title->setTextColor(Color4B(92, 200, 200, 255));
    title->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
    title->setPosition(Vec2(origin.x + 16.0f, origin.y + visibleSize.height - 16.0f));
    addChild(title);

    const auto info = Label::createWithTTF("Left-click fills, right-click marks, R to restart", "fonts/arial.ttf", 18.0f);
    info->setTextColor(Color4B(207, 212, 224, 255));
    info->setPosition(Vec2(origin.x + visibleSize.width / 2.0f, origin.y + 48.0f));
    addChild(info);

    initGrid(origin, visibleSize);
    initWinPanel(origin, visibleSize);
    
    initKeyboardListener();
    initMouseListener();

    resetGrid();

    return true;
}

void NonogramScene::initGrid(const Vec2 &origin, const Size &visibleSize)
{
    const float xOffset = origin.x + (visibleSize.width - (GRID_COLS * Cell::CELL_SIZE)) / 2.0f;
    const float yOffset = origin.y + (visibleSize.height - (GRID_ROWS * Cell::CELL_SIZE)) / 2.0f;

    for (int row = 0; row < GRID_ROWS; ++row)
    {
        std::vector<Cell*> gridRow;
        for (int col = 0; col < GRID_COLS; ++col)
        {
            auto cell = Cell::create();
            cell->setPosition(Vec2(xOffset + col * Cell::CELL_SIZE, yOffset + row * Cell::CELL_SIZE));
            addChild(cell);
            gridRow.push_back(cell);
        }
        _grid.push_back(gridRow);
    }

    const Color4F linesColor(0.333f, 0.376f, 0.478f, 1.0f);
    const auto lines = DrawNode::create();
    constexpr float thickness = 2.0f;
    lines->drawLine(Vec2(0, 200.0f), Vec2(400.0f, 200.0f), linesColor, thickness);
    lines->drawLine(Vec2(200.0f, 0.0f), Vec2(200.0f, 400.0f), linesColor, thickness);
    lines->drawRect(Vec2(0, 0), Vec2(400.0f, 400.0f), linesColor, thickness);
    lines->setPosition(Vec2(xOffset, yOffset));
    addChild(lines);

    const Color4B hintColor(207, 212, 224, 255);
    const float rowOffset = xOffset - 16.0f;
    for (int row = 0; row < GRID_ROWS; ++row)
    {
        _rowHints.push_back(Label::createWithTTF("0", "fonts/arial.ttf", 20.0f));
        _rowHints.back()->setTextColor(hintColor);
        _rowHints.back()->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
        _rowHints.back()->setPosition(Vec2(rowOffset, yOffset + row * Cell::CELL_SIZE + Cell::CELL_SIZE / 2.0f));
        addChild(_rowHints.back());
    }
    const float colOffset = yOffset + 16.0f + Cell::CELL_SIZE * GRID_ROWS;
    for (int col = 0; col < GRID_COLS; ++col)
    {
        _colHints.push_back(Label::createWithTTF("0", "fonts/arial.ttf", 20.0f));
        _colHints.back()->setTextColor(hintColor);
        _colHints.back()->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
        _colHints.back()->setPosition(Vec2(xOffset + col * Cell::CELL_SIZE + Cell::CELL_SIZE / 2.0f, colOffset));
        addChild(_colHints.back());
    }

    _solution = {
        { 0, 0, 0, 0, 1, 1, 0, 0, 0, 0 },
        { 0, 0, 0, 1, 1, 1, 1, 0, 0, 0 },
        { 0, 0, 1, 1, 1, 1, 1, 1, 0, 0 },
        { 0, 1, 1, 1, 1, 1, 1, 1, 1, 0 },
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
        { 0, 1, 1, 1, 1, 1, 1, 1, 1, 0 },
        { 0, 0, 1, 1, 0, 0, 1, 1, 0 ,0 },
        { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }
    };
}

void NonogramScene::initWinPanel(const Vec2 &origin, const Size &visibleSize)
{
    _winPanel = LayerColor::create(Color4B(16, 19, 26, 255), 400.0f, 400.0f);
    _winPanel->setPosition(origin.x + visibleSize.width / 2.0f - 200.0f, origin.y + visibleSize.height / 2.0f - 200.0f);
    _winPanel->setVisible(false);
    addChild(_winPanel);

    const Size& panelSize = _winPanel->getContentSize();
    const auto winLabel = Label::createWithTTF("SOLVED", "fonts/arial.ttf", 40.0f);
    winLabel->setTextColor(Color4B(92, 200, 200, 255));
    winLabel->setPosition(Vec2(panelSize.width / 2.0f, panelSize.height / 2.0f));
    _winPanel->addChild(winLabel);

    const auto restartLabel = Label::createWithTTF("Press R to restart", "fonts/arial.ttf", 18.0f);
    restartLabel->setTextColor(Color4B(207, 212, 224, 255));
    restartLabel->setPosition(Vec2(panelSize.width / 2.0f, panelSize.height / 2.0f - 50.0f));
    _winPanel->addChild(restartLabel);
}

void NonogramScene::initKeyboardListener()
{
    _keyboardListener = EventListenerKeyboard::create();
    _keyboardListener->onKeyPressed = [this](const EventKeyboard::KeyCode keyCode, Event* event) {
        if (keyCode == EventKeyboard::KeyCode::KEY_R)
            restart(); 
    };
    _eventDispatcher->addEventListenerWithSceneGraphPriority(_keyboardListener, this);
}

void NonogramScene::initMouseListener()
{
    _mouseListener = EventListenerMouse::create();
    _mouseListener->onMouseDown = [this](const EventMouse* event) { return onMouseDown(event); };
    _eventDispatcher->addEventListenerWithSceneGraphPriority(_mouseListener, this);
}

bool NonogramScene::onMouseDown(const EventMouse *event)
{
    if (const auto mouseButton = event->getMouseButton();
        mouseButton == EventMouse::MouseButton::BUTTON_LEFT || mouseButton == EventMouse::MouseButton::BUTTON_RIGHT) {
        const Vec2& clickPos = event->getLocation();
        for (int row = 0; row < GRID_ROWS; ++row) {
            for (int col = 0; col < GRID_COLS; ++col) {
                if (const auto cell = _grid[row][col]; cell->getBoundingBox().containsPoint(clickPos))
                {
                    if (mouseButton == EventMouse::MouseButton::BUTTON_LEFT) {
                        cell->leftClick();
                        if (checkSolved())
                        {
                            _winPanel->setVisible(true);
                            _mouseListener->setEnabled(false);
                        }
                    }
                    else if (mouseButton == EventMouse::MouseButton::BUTTON_RIGHT) {
                        cell->rightClick();
                    }

                    return true;
                }
            }
        }
    }

    return false;
}

void NonogramScene::resetGrid() const
{
    for (int row = 0; row < GRID_ROWS; ++row)
    {
        for (int col = 0; col < GRID_COLS; ++col)
        {
            _grid[row][col]->setState(CellState::Empty);
        }
    }

    const std::string rows[10] = { "2", "4", "6", "8", "10", "10", "10", "8", "2 2", "0" };
    for (int i = 0; i < GRID_ROWS; ++i)
    {
        _rowHints[i]->setString(rows[i]);
    }

    const std::string cols[10] = { "3", "5", "7", "8", "8", "8", "8", "7", "5", "3" };
    for (int i = 0; i < GRID_COLS; ++i)
    {
        _colHints[i]->setString(cols[i]);
    }
}

void NonogramScene::restart() const
{
    _winPanel->setVisible(false);
    _mouseListener->setEnabled(true);
    resetGrid();
}

bool NonogramScene::checkSolved()
{
    for (int row = 0; row < GRID_ROWS; ++row)
    {
        for (int col = 0; col < GRID_COLS; ++col)
        {
            const CellState state = _grid[row][col]->getState();
            if (const bool solutionValue = _solution[row][col];
                (state == CellState::Filled && !solutionValue) || (state != CellState::Filled && solutionValue))
            {
                return false;
            }
        }
    }

    return true;
}
