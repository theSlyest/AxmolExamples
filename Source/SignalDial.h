#pragma once

#include "2d/DrawNode.h"
#include "2d/Label.h"
#include "2d/Layer.h"

class SignalDial : public ax::DrawNode
{
public:
    bool init() override;
    
    // implement the "static create()" method manually
    CREATE_FUNC(SignalDial);

    // Show the tuning arrow at a position between 0 (left end) and 1 (right end).
    void setValue(float value);

    void lockTo(const float amount) const { _lockBar->setScaleX(amount); }

private:
    static constexpr float DIAL_HEIGHT = 160.0f;
    static constexpr float DIAL_WIDTH = 840.0f;

    static constexpr float RULER_Y_OFFSET = 20.0f;
    static constexpr float RULER_HEIGHT = 40.0f;
    static constexpr float SMALL_WIDTH = 1.0f;
    static constexpr float LARGE_WIDTH = 2.0f;
    static constexpr float RULER_FONT = 22.0f;

    static constexpr float LABEL_Y = 80.0f;

    static constexpr float ARROW_HEIGHT = 160.0f;
    static constexpr float ARROW_WIDTH = 160.0f;
    static constexpr float ARROW_THICKNESS = 9.0f;
    static constexpr float ARROW_RECT_HEIGHT = 40.0f;
    static constexpr float ARROW_RECT_LINE = 3.0f;
    static constexpr float ARROW_FONT = 26.0f;

    static inline const ax::Color4B COLOR = ax::Color4B(0xff, 0xc2, 0x4a, 255);

    ax::DrawNode* _arrow = nullptr;
    ax::LayerColor* _lockBar = nullptr;
    ax::Label* _label        = nullptr;
    int _shownPercent = -1; // the number currently on the label

    void initArrow();
};
