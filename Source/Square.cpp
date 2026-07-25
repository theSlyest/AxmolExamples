#include "Square.h"

USING_NS_AX;

const std::unordered_map<SquareType, ax::Color4F> Square::Colors = {
    { SquareType::Start,  Color4F(0.0f, 1.0f, 0.0f, 1.0f) }, // Green
    { SquareType::Target, Color4F(1.0f, 1.0f, 0.0f, 1.0f) }, // Yellow
    { SquareType::Empty,  Color4F(0.3f, 0.3f, 0.3f, 1.0f) }, // Dark gray
    { SquareType::Wall,   Color4F(0.7f, 0.7f, 0.7f, 1.0f) }, // Light gray
    { SquareType::Path,   Color4F(0.0f, 0.0f, 1.0f, 1.0f) } // Blue
};

Square* Square::create(const SquareType type)
{
    if (auto* pRet = new (std::nothrow) Square(); pRet && pRet->init(type))
    {
        pRet->autorelease();
        return pRet;
    }
    else
    {
        delete pRet;
        pRet = nullptr;
        return nullptr;
    }
}

// on "init" you need to initialize your instance
bool Square::init(SquareType type)
{
    //////////////////////////////
    // 1. super init first
    if ( !DrawNode::init() )
        return false;

    setType(type);

    return true;
}

void Square::setType(const SquareType type)
{
    clear();
    _type = type;
    drawSolidRect(Vec2(0.0f, 0.0f), Vec2(SQUARE_SIZE, SQUARE_SIZE), Colors.at(type));
    drawRect(Vec2(0.0f, 0.0f), Vec2(SQUARE_SIZE, SQUARE_SIZE), Color4F::BLACK);
}

void Square::switchType()
{
    if (_type == SquareType::Empty)
        setType(SquareType::Wall);
    else if (_type == SquareType::Wall)
        setType(SquareType::Empty);
}