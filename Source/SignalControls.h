#pragma once

#include <functional>
#include <vector>

#include "2d/Node.h"
#include "EventListenerKeyboard.h"

// Translates the keyboard into tuning commands. It knows nothing about the
// dial or the rules: the scene reads the direction and reacts to onRestart.
class SignalControls : public ax::Node
{
public:
    bool init() override;

    CREATE_FUNC(SignalControls);

    // Fired when R is pressed.
    std::function<void()> onRestart;

    // -1 to tune down (left, A), +1 to tune up (right, D), 0 for none. When
    // both are held, the one pressed last wins; releasing it hands control
    // back to the other. Keys that are held stay counted across a restart.
    [[nodiscard]] int getDirection() const;

private:
    // Tuning keys currently held, oldest first.
    std::vector<ax::EventKeyboard::KeyCode> _held;

    static int directionOf(ax::EventKeyboard::KeyCode key);

    void onKeyPressed(ax::EventKeyboard::KeyCode key);
    void onKeyReleased(ax::EventKeyboard::KeyCode key);
};
