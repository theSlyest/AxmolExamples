#include "PegSolitaireScene.h"

USING_NS_AX;

Scene* PegSolitaireScene::createScene()
{
    return PegSolitaireScene::create();
}

bool PegSolitaireScene::init()
{
    if ( !Scene::init() )
    {
        return false;
    }

    const auto& size = Director::getInstance()->getVisibleSize();
    const Vec2& origin = Director::getInstance()->getVisibleOrigin();

    const auto bg = LayerColor::create(Color4B(0x14, 0x18, 0x1f, 0xff));
    addChild(bg);

    const auto title = Label::createWithTTF("PEG SOLITAIRE", "fonts/arial.ttf", 32.0f);
    title->setTextColor(Color4B(0xff, 0xc2, 0x4a, 0xff));
    title->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
    title->setPosition(origin.x + UI_OFFSET_X, origin.y + size.height - UI_OFFSET_Y);
    addChild(title);

    const auto help = Label::createWithTTF("Tap a peg, then tap an empty hole two spaces away to jump. Clear one peg.R to restart", "fonts/arial.ttf", 18.0f);
    help->setPosition(origin.x + size.width / 2.0f, origin.y + UI_OFFSET_Y);
    addChild(help);

    initPegs(origin, size);
    initBoard(origin, size);
    initEndPanel(origin, size);

    initKeyboardListener();
    initTouchListener();

    reset();

    return true;
}

void PegSolitaireScene::initPegs(const Vec2 &origin, const Size &size)
{
    _pegsLbl = Label::createWithTTF("Pegs: 32", "fonts/arial.ttf", 24.0f);
    _pegsLbl->setAnchorPoint(Vec2::ANCHOR_BOTTOM_RIGHT);
    _pegsLbl->setPosition(origin.x + size.width - UI_OFFSET_X, origin.y + size.height - UI_OFFSET_Y);
    addChild(_pegsLbl);
}

void PegSolitaireScene::initBoard(const Vec2 &origin, const Size &size)
{
    _board = PegBoard::create();
    _board->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    _board->setPosition(origin.x + size.width / 2.0f, origin.y + size.height / 2.0f);
    addChild(_board);
}

void PegSolitaireScene::initEndPanel(const ax::Vec2 &origin, const ax::Size &size)
{
    _endPanel = LayerColor::create(Color4B(0, 0, 0, 160), PegBoard::SIZE, PegBoard::SIZE);
    _endPanel->setPosition(origin.x + (size.width - PegBoard::SIZE) / 2.0f, origin.x + (size.height - PegBoard::SIZE) / 2.0f);
    _endPanel->setVisible(false);
    addChild(_endPanel);

    const float middle = PegBoard::SIZE / 2.0f;
    _endLbl = Label::createWithTTF("SOLVED", "fonts/arial.ttf", 28.0f);
    _endLbl->setPosition(middle, middle + 28.0f);
    _endPanel->addChild(_endLbl);

    auto restart = Label::createWithTTF("Press R to restart", "fonts/arial.ttf", 28.0f);
    restart->setPosition(middle, middle - 28.0f);
    _endPanel->addChild(restart);
}

void PegSolitaireScene::initKeyboardListener()
{
    _keyboardListener = EventListenerKeyboard::create();
    _keyboardListener->onKeyPressed = [this](EventKeyboard::KeyCode key, Event *) { onKeyPressed(key); };
    _eventDispatcher->addEventListenerWithSceneGraphPriority(_keyboardListener, this);
}

void PegSolitaireScene::initTouchListener()
{
    _touchListener = EventListenerTouchOneByOne::create();
    _touchListener->onTouchBegan = [this](Touch* touch, Event*) { return onTouch(touch); };
    _eventDispatcher->addEventListenerWithSceneGraphPriority(_touchListener, this);
}

void PegSolitaireScene::setPegs(int pegs)
{
    _pegs = pegs;
    _pegsLbl->setString("Pegs: " + std::to_string(pegs));
}

void PegSolitaireScene::endGame(bool solved)
{
    _endLbl->setString(solved ? "SOLVED" : "NO MOVES");
    _endPanel->setVisible(true);
}

void PegSolitaireScene::reset()
{
    setPegs(32);
    _board->reset();
    _selected.first = -1;
    _endPanel->setVisible(false);
}

void PegSolitaireScene::onKeyPressed(EventKeyboard::KeyCode key)
{
    if (key == EventKeyboard::KeyCode::KEY_R)
        reset();
}

bool PegSolitaireScene::onTouch(Touch *touch)
{
    if (_endPanel->isVisible())
        return true;

    const PegCoords coords = _board->onTouch(touch);
    if (coords.first < 0)
        return false;

    if (_selected.first < 0) {
        if (!_board->hasPeg(coords))
            return true;

        _selected = coords;
        _board->setSelected(coords, true);
    }
    else if (coords == _selected) {
        _selected.first = -1;
        _board->setSelected(coords, false);
    }
    else if (isValidMove(coords)) {
        _board->setPeg(_selected, false);
        _board->setPeg(coords, true);
        _board->setPeg({ (coords.first + _selected.first) / 2, (coords.second + _selected.second) / 2 }, false);
        _selected.first = -1;
        setPegs(_pegs - 1);

        if (_pegs == 1)
            endGame(true);
        else if (!_board->hasJumps())
            endGame(false);
    }

    return true;
}

bool PegSolitaireScene::isValidMove(const PegCoords &coords) const
{
    if (_board->hasPeg(coords))
        return false;

    if (coords.first != _selected.first && coords.second != _selected.second)
        return false;
    
    if (abs(coords.first - _selected.first) + abs(coords.second - _selected.second) != 2)
        return false;

    if (!_board->hasPeg({ (coords.first + _selected.first) / 2, (coords.second + _selected.second) / 2 }))
        return false;

    return true;
}
