#pragma once

#include <functional>

// The rules of the game: tune the dial onto a hidden station and hold it
// there to lock it and score, before the time runs out.
//
// This class has no nodes and no drawing, so it can be read and tested on
// its own. Positions on the dial are numbers between 0 (left end) and 1
// (right end). The scene draws the state and listens to the callbacks.
class SignalLockGame
{
public:
    static constexpr float TUNE_SPEED = 0.3f;   // dial widths per second while a key is held
    static constexpr float START_VALUE = 0.5f;  // the dial starts in the middle
    static constexpr float LOCK_DIST = 0.03f;   // how close counts as being on the station
    static constexpr float LOCK_DELAY = 0.6f;   // seconds on the station to lock it
    static constexpr float LOCK_SAFE = 0.25f;   // a new station is at least this far from the dial
    static constexpr float DETECT_DIST = 0.2f;  // the screen starts to clear up inside this distance
    static constexpr int START_TIME = 45;

    // Returns a random number in [0, 1]. Uses a standard random engine when no source is given.
    using Random = std::function<float()>;

    explicit SignalLockGame(Random random = nullptr);

    // Notifications for the view.
    std::function<void(int)> onScoreChanged;
    std::function<void(int)> onTimeChanged;
    std::function<void()> onGameOver;

    // Get ready for a new round. The clock starts with the first tuning key.
    void reset();

    // `direction` is -1 to tune down, +1 to tune up, 0 to leave the dial alone.
    void update(float dt, int direction);

    [[nodiscard]] bool isPlaying() const { return _playing; }
    [[nodiscard]] bool isOver() const { return _over; }
    [[nodiscard]] int getScore() const { return _score; }
    [[nodiscard]] int getTime() const { return _time; }
    [[nodiscard]] float getDialValue() const { return _dial; }
    [[nodiscard]] float getStation() const { return _station; }
    // How far the lock has progressed, from 0 to 1.
    [[nodiscard]] float getLockProgress() const { return _lockTime / LOCK_DELAY; }
    // How garbled the picture is: 0 on the station, 1 when far from it.
    [[nodiscard]] float getGlitch() const { return _glitch; }

private:
    Random _random;

    bool _playing = false;
    bool _over = false;
    int _score = 0;
    int _time = 0;
    float _timer = 0.0f;
    float _dial = START_VALUE;
    float _station = 0.0f;
    float _lockTime = 0.0f;
    float _glitch = 0.0f;

    void setScore(int score);
    void setTime(int time);
    void newStation();
    void endRound();
};
