#pragma once

#include "2d/Label.h"
#include "2d/Layer.h"

// The translucent "ROUND OVER" overlay with the final score and a restart hint.
class RoundOverLayer : public ax::LayerColor
{
public:
    bool init() override;

    CREATE_FUNC(RoundOverLayer);

    void show(int score);
    void hide() { setVisible(false); }

private:
    static constexpr float LINE_SPACING = 28.0f;

    ax::Label* _score = nullptr;

    ax::Label* addLabel(const char* text, float offsetY);
};
