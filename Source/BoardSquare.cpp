#include "BoardSquare.h"

#include "Helper.h"

USING_NS_AX;

const Color4F BoardSquare::White = Color4F(0.92f, 0.86f, 0.78f, 1.0f);
const Color4F BoardSquare::Black = Color4F(0.27f, 0.32f, 0.39f, 1.0f);
const Color4F BoardSquare::Light = Color4F(1.0f, 0.73f, 0.37f, 1.0f);
const Color4F BoardSquare::Dark = Color4F(0.95f, 0.57f, 0.18f, 1.0f);

BoardSquare* BoardSquare::create(const bool isBlack)
{
    if (auto* pRet = new (std::nothrow) BoardSquare(); pRet && pRet->init(isBlack))
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
bool BoardSquare::init(const bool isBlack)
{
    if ( !DrawNode::init() )
    {
        return false;
    }

    setContentSize(Size(SQUARE_SIDE, SQUARE_SIDE));

    _isBlack = isBlack;
    const auto color = isBlack ? Black : White;
    drawSolidRect(Vec2::ZERO, Vec2(SQUARE_SIDE, SQUARE_SIDE), color);

    initLegalNode();
    initHoverNode();

    return true;
}

void BoardSquare::initLegalNode()
{
    constexpr Vec2 center(SQUARE_SIDE / 2.0f, SQUARE_SIDE / 2.0f);

    _legalNode = DrawNode::create();
    _legalNode->drawSolidCircle(center, 12.0f, 0.0f, 64, Light);
    _legalNode->drawSolidCircle(center, 10.0f, 0.0f, 64, Dark);
    _legalNode->drawSolidCircle(center, 4.0f, 0.0f, 64, Light);
    _legalNode->setVisible(false);

    addChild(_legalNode);
}

void BoardSquare::initHoverNode()
{
    constexpr float outerOffset = 4.0f;
    constexpr float innerOffset = 7.0f;
    constexpr float outerEnd = SQUARE_SIDE - outerOffset;
    constexpr float innerEnd = SQUARE_SIDE - innerOffset;
    constexpr Vec2 center(SQUARE_SIDE / 2.0f, SQUARE_SIDE / 2.0f);

    _hoverNode = DrawNode::create();
    Helper::drawRoundedRect(_hoverNode, Vec2(outerOffset, outerOffset), Vec2(outerEnd, outerEnd), 10.0f, Light);
    Helper::drawRoundedRect(_hoverNode, Vec2(innerOffset, innerOffset), Vec2(innerEnd, innerEnd), 8.0f, Dark);
    _hoverNode->drawCircle(center, 8.0f, 0.0f, 64, false, Color4F::WHITE);
    _hoverNode->setVisible(false);

    addChild(_hoverNode);
}

void BoardSquare::onMouseOver()
{
    _hoverNode->setVisible(true);
}

void BoardSquare::onMouseOut()
{

}

void BoardSquare::setHovered(const bool hovered)
{
    if (!hovered || _legalNode->isVisible()) {
        _hoverNode->setVisible(hovered);
    }
}
