#pragma once

// The fewest mirrors the puzzle was solved with, saved between runs.
class BestScore
{
public:
    // `startValue` is the best until the puzzle is solved with fewer mirrors than that.
    explicit BestScore(int startValue);

    [[nodiscard]] int get() const { return _best; }

    // Keep `mirrors` if it beats the best (fewer is better). Returns true if it did.
    bool submit(int mirrors);

private:
    int _best;
};
