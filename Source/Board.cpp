#include "Board.h"

USING_NS_AX;

// on "init" you need to initialize your instance
bool Board::init()
{
    if ( !DrawNode::init() )
    {
        return false;
    }

    constexpr auto side = 608.0f;
    setContentSize(Size(side, side));

    // Border
    drawSolidRect(Vec2::ZERO, Vec2(side, side), Color4F(0.16f, 0.18f, 0.24f, 1.0f));
    
    // Squares
    _squares.assign(8, {});
    for (int c = 0; c < 8; ++c) {
        bool isBlack = c % 2 == 0;
        for (int r = 0; r < 8; ++r) {
            constexpr float offset = 16.0f;
            auto square = BoardSquare::create(isBlack);
            square->setPosition(SQUARE_SIDE * c + offset, SQUARE_SIDE * r + offset);
            addChild(square);
            _squares[r].push_back(square);
            isBlack = !isBlack;
        }
    }

    return true;
}
