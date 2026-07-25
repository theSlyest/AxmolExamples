#pragma once

#include <chrono>
#include <deque>

class InputRecorder
{
public:
    struct Input {
        int keyCode;
        bool isPressed;
        std::chrono::nanoseconds delay;
    };

    ~InputRecorder();

    void start();
    void record(int keyCode, bool isPressed);

    void restart();
    std::pair<bool, Input> readRecord();

    void reset();

private:
    std::chrono::steady_clock::time_point _startTime;
    std::deque<Input> _inputs;
    std::deque<Input> _replayInputs;
};
