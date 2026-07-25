#include "InputRecorder.h"

InputRecorder::~InputRecorder()
{
    _inputs.clear();
}

void InputRecorder::start()
{
    _startTime = std::chrono::steady_clock::now();
}

void InputRecorder::record(const int keyCode, const bool isPressed)
{
    const auto& elapsed = std::chrono::steady_clock::now() - _startTime;
    _inputs.push_back({ .keyCode = keyCode, .isPressed = isPressed, .delay = elapsed });
}

void InputRecorder::restart()
{
    _replayInputs.clear();
    _inputs.swap(_replayInputs);
    start();
}

std::pair<bool, InputRecorder::Input> InputRecorder::readRecord()
{
    if (_replayInputs.empty())
        return { false, {} };

    if (const auto& elapsed = std::chrono::steady_clock::now() - _startTime; _replayInputs[0].delay > elapsed)
        return { false, {} };

    auto res = _replayInputs[0];
    _replayInputs.pop_front();
    return { true, res };    
}

void InputRecorder::reset()
{
    _replayInputs.clear();
    _inputs.clear();
}