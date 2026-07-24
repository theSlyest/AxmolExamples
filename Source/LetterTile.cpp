#include "LetterTile.h"

USING_NS_AX;

const Color4F LetterTile::Yellow(0.95f, 0.76f, 0.21f, 1.0f);
const Color4F LetterTile::Green(0.48f, 0.72f, 0.32f, 1.0f);

// on "init" you need to initialize your instance
bool LetterTile::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Node::init() )
    {
        return false;
    }

    _letter = 0;
    
    _background = DrawNode::create();
    const float borderOffset = BorderWidth / 2.0f;
    _background->drawRect(Vec2(borderOffset, borderOffset), Vec2(SIZE - borderOffset, SIZE - borderOffset), Color4F::GRAY, BorderWidth);
    addChild(_background);

    _label = Label::createWithTTF("", "fonts/arial.ttf", LabelFontSize);
    _label->setPosition(SIZE / 2.0f, SIZE / 2.0f);
    addChild(_label);

    setContentSize(Size(SIZE, SIZE));

    setState(TileState::DEFAULT);

    return true;
}

void LetterTile::erase()
{
    _letter = 0;
    _label->setString("");
}

void LetterTile::setLetter(char letter)
{
    _letter = letter;
    _label->setString(std::string(1, letter));
}

void LetterTile::setState(TileState state)
{
    Vec2 to(SIZE, SIZE);
    _state = state;
    _background->clear();
    switch (state) {
        case TileState::GREEN:
            _background->drawSolidRect(Vec2::ZERO, to, Green);
            break;
        case TileState::YELLOW:
            _background->drawSolidRect(Vec2::ZERO, to, Yellow);
            break;
        default:
            _background->drawRect(Vec2(1.0f, 1.0f), Vec2(SIZE - 1.0f, SIZE - 1.0f), Color4F::GRAY);
            break;
    }
}

