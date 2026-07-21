#include "Brick.h"

using namespace ax;

const Size Brick::BrickSize(66.0f, 24.0f);
const Color4F Brick::StrongColor(1.0f, 0.3f, 0.0f, 1.0f); // Orange
const Color4F Brick::WeakColor(1.0f, 0.6f, 0.0f, 1.0f); // Orange
const Color4F Brick::NormalColor(0.0f, 1.0f, 0.0f, 1.0f); // Green

Brick* Brick::create(bool isStrong)
{
    Brick *pRet = new(std::nothrow) Brick();
    if (pRet && pRet->init(isStrong))
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

bool Brick::init(bool isStrong)
{
    //////////////////////////////
    // 1. super init first
    if ( !FilledRect::init(BrickSize, isStrong ? StrongColor : NormalColor) )
    {
        return false;
    }

    _isStrong = isStrong;
    _isOk = isStrong;

    return true;
}

bool Brick::hit()
{
    if (_isOk) {
        _isOk = false;
        setFillColor(WeakColor);
        return false;
    }

    return true;
}