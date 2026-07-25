#include "MoveRecorder.h"

MoveRecorder::~MoveRecorder()
{
    
}

void MoveRecorder::startRecording()
{
    _changes.clear();
    _startTime = std::chrono::steady_clock::now();
}

void MoveRecorder::record(const ax::Vec2 &direction)
{
    _changes.push_back({ direction, std::chrono::steady_clock::now() - _startTime });
}

void MoveRecorder::startReading()
{
    _startReadingTime = std::chrono::steady_clock::now();
}

bool MoveRecorder::read(ax::Vec2& direction)
{
    if (_changes.empty())
        return false;
    
    if (_changes[0].elapsed > std::chrono::steady_clock::now() - _startReadingTime) 
        return false;

    direction = _changes[0].direction;
    _changes.pop_front();
    return true;
}

bool MoveRecorder::skip(ax::Vec2 &direction)
{
    if (_changes.empty())
        return false;

    _startReadingTime += _changes[0].elapsed - (std::chrono::steady_clock::now() - _startReadingTime);
    direction = _changes[0].direction;
    _changes.pop_front();
    return true;
}
