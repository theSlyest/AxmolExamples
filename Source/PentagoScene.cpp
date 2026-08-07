#include "PentagoScene.h"

#include "PentagoBlock.h"
#include "Director.h"
#include "EventDispatcher.h"
#include "EventListenerKeyboard.h"
#include "EventListenerTouch.h"

USING_NS_AX;

// on "init" you need to initialize your instance
bool PentagoScene::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Scene::init() )
        return false;

    const Vec2& size = _director->getVisibleSize();
    const Vec2& origin = _director->getVisibleOrigin();

    // Background
    const auto bg = LayerColor::create(Color4B(0x17, 0x1b, 0x22, 0xff));
    addChild(bg);

    // Title
    const auto title = Label::createWithTTF("PENTAGO", "fonts/arial.ttf", 36.0f);
    title->setTextColor(Color4B(0x43, 0xc6, 0xb8, 0xff));
    title->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
    title->setPosition(origin.x + 16.0f, origin.y + size.y - 16.0f);
    addChild(title);

    // Help line
    const auto help = Label::createWithTTF("Click a cell to drop, then a twist arrow, R to restart", "fonts/arial.ttf", 18.0f);
    help->setPosition(origin.x + size.x / 2.0f, origin.y + 48.0f);
    addChild(help);

    // Turn line
    _turnLbl = Label::createWithTTF("White to place", "fonts/arial.ttf", 28.0f);
    _turnLbl->setPosition(origin.x + size.x / 2.0f, 88.0f);
    addChild(_turnLbl);

    _board = PentagoBoard::create();
    _board->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM),
    _board->setPosition(origin.x + size.x / 2.0f, origin.y + 144.0f);
    addChild(_board);

    initEndLayer(origin, size);

    const auto keyListener = EventListenerKeyboard::create();
    keyListener->onKeyPressed = [this](const EventKeyboard::KeyCode key, Event *) { onKeyPressed(key); };
    _eventDispatcher->addEventListenerWithSceneGraphPriority(keyListener, this);

    const auto touchListener = EventListenerTouchOneByOne::create();
    touchListener->onTouchBegan = [this](Touch* touch, Event*) { return onTouch(touch); };
    _eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);

    _eventDispatcher->addCustomEventListener(PentagoBlock::TURNED_EVENT, [this](EventCustom*) { onBlockTurned(); });

    reset();

    return true;
}

void PentagoScene::initEndLayer(const Vec2& origin, const Vec2& size)
{
    constexpr Vec2 endSize(320.0f, 120.0f);
    _endLayer = LayerColor::create(Color4B(0x10, 0x13, 0x1a, 0xff), endSize.x, endSize.y);
    _endLayer->setPosition(origin.x + (size.x - endSize.x) / 2.0f, origin.y + (size.y - endSize.y) / 2.0f);
    _endLayer->setVisible(false);
    addChild(_endLayer, 1);

    _endLbl = Label::createWithTTF("WHITE WINS!", "fonts/arial.ttf", 36.0f);
    _endLbl->setTextColor(Color4B(0xe6, 0xe6, 0xee, 0xff));
    _endLbl->setPosition(endSize.x / 2.0f, endSize.y / 2.0f + 18.0f);
    _endLayer->addChild(_endLbl);

    const auto restartLbl = Label::createWithTTF("Press R to restart", "fonts/arial.ttf", 18.0f);
    restartLbl->setTextColor(Color4B(0xcf, 0xd4, 0xe0, 0xff));
    restartLbl->setPosition(endSize.x / 2.0f, endSize.y / 2.0f - 24.0f);
    _endLayer->addChild(restartLbl);
}

void PentagoScene::reset()
{
    _isBlack = false;
    _board->reset();
    PentagoBlock::setEnabled(true);
    PentagoBlock::setTurning(false);
    _endLayer->setVisible(false);
    updateTurnLabel();
}

void PentagoScene::onKeyPressed(const EventKeyboard::KeyCode key)
{
    if (key == EventKeyboard::KeyCode::KEY_R)
        reset();
}

bool PentagoScene::onTouch(Touch *touch) const
{
    if (!PentagoBlock::isEnabled() || PentagoBlock::isTurning())
        return false;

    if (_board->onTouch(touch, _isBlack)) {
        PentagoBlock::setTurning(true);
        updateTurnLabel();
        return true;
    }

    return false;
}

void PentagoScene::onBlockTurned()
{
    if (_board->checkLines()) {
        if (const LinedSockets lines = _board->getLines(); lines.contains(true) && lines.contains(false))
            showEndLayer(0);
        else {
            _board->ringLines();
            if (lines.contains(true))
                showEndLayer(1);
            else
                showEndLayer(-1);
        }
        return;
    }
    PentagoBlock::setTurning(false);
    _isBlack = !_isBlack;
    updateTurnLabel();
}

void PentagoScene::updateTurnLabel() const
{
    std::string text = _isBlack ? "Black to " : "White to ";
    text += PentagoBlock::isTurning() ? "twist" : "place";
    _turnLbl->setString(text);
}

void PentagoScene::showEndLayer(const int result) const
{
    if (result < 0)
        _endLbl->setString("WHITE WINS!");
    else if (result > 0)
        _endLbl->setString("BLACK WINS!");
    else
        _endLbl->setString("DRAW!");

    PentagoBlock::setEnabled(false);
    _endLayer->setVisible(true);
}
