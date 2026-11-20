#include "BestScore.h"

#include "UserDefault.h"

namespace {
constexpr auto KEY = "BEST";
}

BestScore::BestScore(const int startValue) : _best(ax::UserDefault::getInstance()->getIntegerForKey(KEY, startValue))
{
}

bool BestScore::submit(const int mirrors)
{
    if (mirrors >= _best)
        return false;

    _best = mirrors;
    const auto userDefault = ax::UserDefault::getInstance();
    userDefault->setIntegerForKey(KEY, mirrors);
    userDefault->flush();
    return true;
}
