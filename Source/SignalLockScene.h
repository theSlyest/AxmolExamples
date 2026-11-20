#pragma once

#include "2d/Scene.h"
#include <memory>

#include "SignalControls.h"
#include "SignalDial.h"
#include "SignalLockGame.h"
#include "SignalScreen.h"

class SignalLockScene : public ax::Scene
{
public:
    bool init() override;
    
    // implement the "static create()" method manually
    CREATE_FUNC(SignalLockScene);

    void update(float dt) override;

private:
    // Layout, in pixels.
    static constexpr float LABELS_OFFSET = 20.0f;
    static constexpr float DIAL_BOTTOM = 40.0f;
    static constexpr float HELP_BOTTOM = 240.0f;
    static constexpr float SCREEN_BOTTOM = 288.0f;

    static inline const ax::Color4B UI_COLOR = ax::Color4B(0xff, 0xc2, 0x4a, 255);

    std::unique_ptr<SignalLockGame> _game;

    ax::Label* _scoreLabel = nullptr;
    ax::Label* _endScoreLabel = nullptr;
    ax::Label* _timeLabel = nullptr;
    ax::LayerColor* _endLayer = nullptr;
    SignalControls* _controls = nullptr;

    SignalDial* _dial = nullptr;
    SignalScreen* _screen = nullptr;

    void initEndLayer(const ax::Size& size);
    void initGame();

    void reset();
    void gameOver();

    // Copy the game's state onto the dial and the screen.
    void syncViews(float dt) const;
};
