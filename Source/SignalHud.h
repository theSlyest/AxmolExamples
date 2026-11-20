#pragma once

#include "2d/Label.h"
#include "2d/Node.h"

// The score (top left) and the time left (top right).
class SignalHud : public ax::Node
{
public:
    // The HUD covers an area of the given size; place the node at the visible origin.
    static SignalHud* create(const ax::Vec2& size);

    bool init(const ax::Vec2& size);

    void setScore(int score) const;
    void setTime(int seconds) const;

private:
    static constexpr float OFFSET = 20.0f;
    static constexpr float FONT_SIZE = 24.0f;
    static inline const ax::Color4B COLOR = ax::Color4B(0xff, 0xc2, 0x4a, 255);

    ax::Label* _score = nullptr;
    ax::Label* _time = nullptr;

    ax::Label* addLabel(const char* text, const ax::Vec2& anchor, const ax::Vec2& position);
};
