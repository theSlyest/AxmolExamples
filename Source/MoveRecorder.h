#pragma once

#include <chrono>
#include <deque>

#include "axmol.h"

class MoveRecorder 
{
public:
    struct MoveChange {
        ax::Vec2 direction;
        std::chrono::steady_clock::duration elapsed;
    };

    virtual ~MoveRecorder();

    void startRecording();
    void record(const ax::Vec2& direction);

    void startReading();
    bool read(ax::Vec2& direction);
    bool skip(ax::Vec2& direction);

private:
    std::chrono::steady_clock::time_point _startTime;
    std::chrono::steady_clock::time_point _startReadingTime;

    std::deque<MoveChange> _changes;
};
