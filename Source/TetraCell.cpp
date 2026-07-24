#include "TetraCell.h"

USING_NS_AX;

const Color4F TetraCell::BORDER_COLOR(0.0f, 0.0f, 0.0f, 1.0f);

const std::map<Tetromino, std::pair<ax::Color4F, ax::Color4F>> TetraCell::COLORS {
    { Tetromino::I, { Color4F(0.37f, 0.79f, 0.85f, 1.0f), Color4F(0.6f, 0.84f, 0.89f, 1.0f) } },
    { Tetromino::O, { Color4F(0.86f, 0.84f, 0.11f, 1.0f), Color4F(0.97f, 0.93f, 0.54f, 1.0f) } },
    { Tetromino::T, { Color4F(0.65f, 0.22f, 0.58f, 1.0f), Color4F(0.8f, 0.41f, 0.57f, 1.0f) } },
    { Tetromino::S, { Color4F(0.39f, 0.74f, 0.28f, 1.0f), Color4F(0.49f, 0.78f, 0.44f, 1.0f) } },
    { Tetromino::Z, { Color4F(0.93f, 0.2f, 0.18f, 1.0f), Color4F(0.95f, 0.36f, 0.36f, 1.0f) } },
    { Tetromino::L, { Color4F(0.95f, 0.45f, 0.2f, 1.0f), Color4F(0.97f, 0.59f, 0.2f, 1.0f) } },
    { Tetromino::J, { Color4F(0.13f, 0.33f, 0.65f, 1.0f), Color4F(0.17f, 0.41f, 0.7f, 1.0f) } }
};

bool TetraCell::init(Tetromino mino)
{
    if ( !DrawNode::init() )
    {
        return false;
    }

    setContentSize(Size(SIZE, SIZE));
    setTetromino(mino);
    
    return true;
}

TetraCell *TetraCell::create(Tetromino mino)
{
    TetraCell *pRet = new(std::nothrow) TetraCell();
    if (pRet && pRet->init(mino))
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

void TetraCell::setTetromino(Tetromino mino)
{
    clear();
    drawSolidRect(Vec2::ZERO, Vec2(_contentSize), COLORS.at(mino).first);
    drawRect(Vec2::ZERO, Vec2(_contentSize), BORDER_COLOR, 1.0f);
    const float offset = 2.0f;
    drawRect(Vec2(offset, offset), Vec2(SIZE - offset, SIZE - offset), COLORS.at(mino).second, 1.0f);
}
