#pragma once

#include "2d/Node.h"
#include "HudValue.h"

// The three boxes along the top of the screen: the mirrors used (top left),
// the mirrors remaining (top center) and the best result (top right).
class LaserHud : public ax::Node
{
public:
    // The HUD covers an area of the given size; place the node at the visible origin.
    static LaserHud* create(const ax::Vec2& size);

    bool init(const ax::Vec2& size);

    void setMirrors(int mirrors) const;
    void setRemaining(int remaining) const;
    void setBest(int best) const;

private:
    static constexpr float TOP = 32.0f;     // space above the boxes
    static constexpr float MARGIN = 64.0f;  // space between the side boxes and the screen edge

    HudValue* _mirrors = nullptr;
    HudValue* _remaining = nullptr;
    HudValue* _best = nullptr;
};
