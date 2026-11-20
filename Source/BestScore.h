#pragma once

#include <functional>

// The fewest mirrors the puzzle was solved with.
//
// Where the number is kept is injected, so the rule can be tested without
// touching the saved settings; saved() gives the real thing.
class BestScore
{
public:
    // Reads the stored best result, falling back to the given start value.
    using Load = std::function<int(int startValue)>;
    using Save = std::function<void(int best)>;

    // `startValue` is the best until the puzzle is solved with fewer mirrors than that.
    BestScore(int startValue, const Load& load, Save save);

    // A best score saved between runs.
    static BestScore saved(int startValue);

    [[nodiscard]] int get() const { return _best; }

    // Keep `mirrors` if it beats the best (fewer is better), and save it.
    // Returns true if it did.
    bool submit(int mirrors);

private:
    int _best;
    Save _save;
};
