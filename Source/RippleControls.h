#pragma once

#include <functional>

#include "2d/Node.h"
#include "EventListenerTouch.h"

// Turns the touches and the keyboard into commands. It knows nothing about the
// water or the rules: the scene decides what a touch means.
class RippleControls : public ax::Node
{
public:
    bool init() override;

    CREATE_FUNC(RippleControls);

    // Called for every touch; return true if the touch was used.
    std::function<bool(ax::Touch*)> onTouch;

    // Fired when R is pressed.
    std::function<void()> onRestart;

    // Stop or resume reacting to touches (the keyboard keeps working).
    void setTouchEnabled(bool enabled) const;

private:
    ax::EventListenerTouchOneByOne* _touchListener = nullptr;
};
