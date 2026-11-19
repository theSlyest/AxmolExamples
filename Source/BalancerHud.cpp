#include "BalancerHud.h"

#include "fmt/format.h"

USING_NS_AX;

BalancerHud* BalancerHud::create(const Vec2& size)
{
    if (auto* pRet = new (std::nothrow) BalancerHud(); pRet && pRet->init(size))
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

bool BalancerHud::init(const Vec2& size)
{
    if (!Node::init())
        return false;

    setContentSize(Size(size));

    const float top = size.y - OFFSET;

    _upTime = BalancerValue::create(Color4F(0.2f, 0.6f, 1.0f, 1.0f), "UPTIME");
    _upTime->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
    _upTime->setPosition(OFFSET, top);
    addChild(_upTime);

    _strikes = BalancerValue::create(Color4F(0.4f, 1.0f, 0.4f, 1.0f), "STRIKES");
    _strikes->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    _strikes->setPosition(size.x / 2.0f, top);
    addChild(_strikes);

    _best = BalancerValue::create(Color4F(1.0f, 1.0f, 0.2f, 1.0f), "BEST");
    _best->setAnchorPoint(Vec2::ANCHOR_TOP_RIGHT);
    _best->setPosition(size.x - OFFSET, top);
    addChild(_best);

    return true;
}

void BalancerHud::setUpTime(const float seconds) const
{
    _upTime->setValue(fmt::format("{:03d}s", static_cast<int>(seconds)));
}

void BalancerHud::setStrikes(const int strikes) const
{
    _strikes->setValue(std::to_string(strikes));
}

void BalancerHud::setBest(const int seconds) const
{
    _best->setValue(fmt::format("{:03d}s", seconds));
}

float BalancerHud::getBottom() const
{
    return _upTime->getBoundingBox().getMinY();
}
