#include "Info.h"

#include "Helper.h"
#include "Piece.h"

USING_NS_AX;

const Color4F Info::NormalColor = Color4F(0.42f, 0.5f, 0.62f, 1.0f);
const Color4F Info::AlertColor = Color4F(1.0f, 0.73f, 0.37f, 1.0f);

// on "init" you need to initialize your instance
bool Info::init()
{
    if ( !DrawNode::init() )
    {
        return false;
    }

    const auto transparent = Color4F(0.0f, 0.0f, 0.0f, 0.0f);
    Helper::drawRoundedRect(this, Vec2(110.0f, 0.0f), Vec2(690.0f, 100.0f), 28.0f, transparent, NormalColor);
    Helper::drawRoundedRect(this, Vec2(130.0f, 20.0f), Vec2(230.0f, 80.0f), 16.0f, Color4F(0.03f, 0.06f, 0.09f, 1.0f), NormalColor);
    Helper::drawRoundedRect(this, Vec2(250.0f, 20.0f), Vec2(550.0f, 80.0f), 16.0f, Color4F(0.09f, 0.13f, 0.20f, 1.0f), NormalColor);
    Helper::drawRoundedRect(this, Vec2(570.0f, 20.0f), Vec2(670.0f, 80.0f), 16.0f, Color4F(0.12f, 0.15f, 0.20f, 1.0f), NormalColor);

    const auto blackPiece = Piece::create(true);
    blackPiece->setScale(0.7f);
    blackPiece->setPosition(140.0f, 30.0f);
    addChild(blackPiece);
    const auto whitePiece = Piece::create(false);
    whitePiece->setScale(0.7f);
    whitePiece->setPosition(620.0f, 30.0f);
    addChild(whitePiece);

    const auto blackLabel = Label::createWithTTF("BLACK", "fonts/arial.ttf", 10.0f);
    blackLabel->setTextColor(Color4B(NormalColor));
    blackLabel->setPosition(200.0f, 68.0f);
    addChild(blackLabel);
    const auto whiteLabel = Label::createWithTTF("WHITE", "fonts/arial.ttf", 10.0f);
    whiteLabel->setTextColor(Color4B(NormalColor));
    whiteLabel->setPosition(600.0f, 68.0f);
    addChild(whiteLabel);

    _blackCountLabel = Label::createWithTTF("0", "fonts/arial.ttf", 28.0f);
    _blackCountLabel->setPosition(200.0f, 40.0f);
    addChild(_blackCountLabel);
    _whiteCountLabel = Label::createWithTTF("0", "fonts/arial.ttf", 28.0f);
    _whiteCountLabel->setPosition(604.0f, 40.0f);
    addChild(_whiteCountLabel);

    _turnLabel = Label::createWithTTF("BLACK'S TURN", "fonts/arial.ttf", 22.0f);
    _turnLabel->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
    _turnLabel->setPosition(260.0f, 46.0f);
    addChild(_turnLabel);

    _instructionLabel = Label::createWithTTF("Select an orange legal move", "fonts/arial.ttf", 12.0f);
    _instructionLabel->setTextColor(Color4B(NormalColor));
    _instructionLabel->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
    _instructionLabel->setPosition(260.0f, 32.0f);
    addChild(_instructionLabel);

    Helper::drawRoundedRect(this, Vec2(450.f, 36.0f), Vec2(530.0f, 64.0f), 12.0f, Color4F(0.22f, 0.15f, 0.09f, 1.0f), Color4F(0.95f, 0.57f, 0.18f, 1.0f));

    _moveCountLabel = Label::createWithTTF("0 MOVES", "fonts/arial.ttf", 11.0f);
    _moveCountLabel->setTextColor(Color4B(AlertColor));
    _moveCountLabel->setPosition(490.0f, 50.0f);
    addChild(_moveCountLabel);

    initTurnNode();
    
    return true;
}

void Info::initTurnNode()
{
    _turnNode = DrawNode::create();
    Helper::drawRoundedRect(_turnNode, Vec2::ZERO, Vec2(8.0f, 64.0f), 4.0f, Color4F(0.95f, 0.57f, 0.18f, 1.0f));
    _turnNode->setPosition(248.0f, 18.0f);
    addChild(_turnNode);
}

void Info::setPieceCount(const int val, const bool isBlack) const
{
    if (isBlack) {
        _blackCountLabel->setString(std::to_string(val));
    }
    else {
        _whiteCountLabel->setString(std::to_string(val));
    }
}

void Info::setTurn(const bool isBlack) const
{
    if (isBlack) {
        _turnLabel->setString("BLACK'S TURN");
        _turnNode->setPositionX(248.0f);
    }
    else {
        _turnLabel->setString("WHITE'S TURN");
        _turnNode->setPositionX(544.0f);
    }
}

void Info::setMoveCount(int val) const
{
    const char* format = val > 1 ? "%d MOVES" : "%d MOVE";
    _moveCountLabel->setString(StringUtils::format(format, val));
}

void Info::setInstruction(const std::string& instruction, const bool isAlert) const
{
    _instructionLabel->setString(instruction);
    _instructionLabel->setTextColor(Color4B(isAlert ? AlertColor : NormalColor));
}

void Info::setResult(const int result) const
{
    if (result == 0)
        _turnLabel->setString("DRAW GAME");
    else if (result > 0)
        _turnLabel->setString("BLACK WINS");
    else
        _turnLabel->setString("WHITE WINS");

    setInstruction("Game over.", true);
    _turnNode->setVisible(false);
}