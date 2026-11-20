#pragma once

#include "2d/Label.h"
#include "2d/Layer.h"

// The translucent "ROUND OVER" overlay with the final score and a restart hint.
class SignalGameOverLayer : public ax::LayerColor
{
public:
    bool init() override;

    CREATE_FUNC(SignalGameOverLayer);

    void show(int score);
    void hide() { setVisible(false); }

private:
    ax::Label* _scoreLabel = nullptr;

    ax::Label* addLabel(const char* text, float fontSize, float offsetY);
};
