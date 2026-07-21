#include "FilledRect.h"

using namespace ax;

FilledRect* FilledRect::create(const Size& size, const Color4F& color)
{
    FilledRect *pRet = new(std::nothrow) FilledRect();
    if (pRet && pRet->init(size, color))
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

bool FilledRect::init(const Size& size, const Color4F& color)
{
    if ( !DrawNode::init() )
    {
        return false;
    }

    setContentSize(size);
    drawSolidRect(Vec2::ZERO, Vec2(size), color);

    return true;
}

void FilledRect::setFillColor(const ax::Color4F& color)
{
    clear();
    drawSolidRect(Vec2::ZERO, Vec2(getContentSize()), color);
}