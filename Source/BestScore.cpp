#include "BestScore.h"

#include <utility>

#include "UserDefault.h"

namespace {
constexpr auto KEY = "BEST";
}

BestScore::BestScore(const int startValue, const Load& load, Save save)
    : _best(load(startValue)), _save(std::move(save))
{
}

BestScore BestScore::saved(const int startValue)
{
    return BestScore(
        startValue,
        [](const int start) { return ax::UserDefault::getInstance()->getIntegerForKey(KEY, start); },
        [](const int best) {
            const auto userDefault = ax::UserDefault::getInstance();
            userDefault->setIntegerForKey(KEY, best);
            userDefault->flush();
        });
}

bool BestScore::submit(const int mirrors)
{
    if (mirrors >= _best)
        return false;

    _best = mirrors;
    _save(mirrors);
    return true;
}
