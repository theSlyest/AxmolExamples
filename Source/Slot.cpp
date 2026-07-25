#include "Slot.h"

USING_NS_AX;

Slot *Slot::create(const Ingredient ingredient)
{
    if (auto pRet = new (std::nothrow) Slot(); pRet && pRet->init(ingredient))
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


bool Slot::init(const Ingredient ingredient)
{
    if (!DrawNode::init())
        return false;

    _ingredient = ingredient;
    setContentSize(Size(Radius * 2.0f, Radius *2.0f));
    empty();

    return true;
}

void Slot::fill()
{
    _isFull = true;
    clear();
    drawSolidCircle(Vec2(Radius, Radius), Radius, 0.0f, 64, Color4F(0.0f, 0.0f, 0.0f, 0.4f));
    drawCircle(Vec2(Radius, Radius), Radius - 1.5f, 0.0f, 64, false, Color4F::WHITE, 3.0f);
}

void Slot::empty()
{
    _isFull = false;
    clear();
    drawSolidCircle(Vec2(Radius, Radius), Radius, 0.0f, 64, Color4F(0.0f, 0.0f, 0.0f, 0.2f));
    drawCircle(Vec2(Radius, Radius), Radius - 1.5f, 0.0f, 64, false, Color4F::WHITE, 3.0f);
}
