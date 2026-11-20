#pragma once

#include "2d/Label.h"
#include "2d/Node.h"

// The score (top left) and the timer (top right).
class RippleHud : public ax::Node
{
public:
    // The HUD covers an area of the given size; place the node at the visible origin.
    static RippleHud* create(const ax::Vec2& size);

    bool init(const ax::Vec2& size);

    void setScore(int score) const;
    void setTimer(int seconds) const;

private:
    static constexpr float MARGIN = 16.0f;

    ax::Label* _score = nullptr;
    ax::Label* _timer = nullptr;

    ax::Label* addLabel(const char* text, const ax::Vec2& anchor, const ax::Vec2& position);
};
