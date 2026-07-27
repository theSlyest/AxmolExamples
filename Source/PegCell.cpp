#include "PegCell.h"

USING_NS_AX;

const Color4F PegCell::COLOR(0.02f, 0.04f, 0.08f, 1.0f);
const Color4F PegCell::PEG_COLOR(0.31f, 0.65f, 0.57f, 1.0f);
const Color4F PegCell::RING_COLOR(0.76f, 0.76f, 0.0f, 1.0f);

PegCell* PegCell::create(bool hasPeg)
{
    if (auto* pRet = new (std::nothrow) PegCell(); pRet && pRet->init(hasPeg))
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

void PegCell::setPeg(bool peg) const
{
    _peg->setVisible(peg);

    if (!peg)
        _ring->setVisible(false);
}

void PegCell::setSelected(bool select) const
{
    if (!_peg->isVisible())
        return;

    _ring->setVisible(select);
}

bool PegCell::init(const bool hasPeg)
{
    if ( !DrawNode::init() )
        return false;

    setContentSize(Size(SIZE, SIZE));

    constexpr Vec2 center(SIZE / 2.0f, SIZE / 2.0f);
    drawSolidCircle(center, RADIUS, 0.0f, RADIUS * M_PI * 2.0f, COLOR);

    _peg = DrawNode::create();
    _peg->setContentSize(_contentSize);
    _peg->drawSolidCircle(center, PEG_RADIUS, 0.0f, PEG_RADIUS * M_PI * 2.0f, PEG_COLOR);
    _peg->setVisible(hasPeg);
    addChild(_peg);

    _ring = DrawNode::create();
    _ring->setContentSize(_contentSize);
    _ring->drawCircle(center, RING_RADIUS, 0.0f, RING_RADIUS * M_PI * 2.0f, false, RING_COLOR, RING_WIDTH);
    _ring->setVisible(false);
    addChild(_ring);
    
    return true;
}