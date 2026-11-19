#pragma once

// The best uptime (in seconds), saved between runs.
class BestScore
{
public:
    BestScore();

    [[nodiscard]] int get() const { return _best; }

    // Keep `seconds` if it beats the best score. Returns true if it did.
    bool submit(int seconds);

private:
    int _best = 0;
};
