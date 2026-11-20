#pragma once

#include "2d/Scene.h"
#include <memory>

#include "SignalControls.h"
#include "SignalDial.h"
#include "SignalGameOverLayer.h"
#include "SignalHud.h"
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
    static constexpr float DIAL_BOTTOM = 40.0f;
    static constexpr float HELP_BOTTOM = 240.0f;
    static constexpr float SCREEN_BOTTOM = 288.0f;

    std::unique_ptr<SignalLockGame> _game;

    SignalHud* _hud = nullptr;
    SignalGameOverLayer* _endLayer = nullptr;
    SignalControls* _controls = nullptr;

    SignalDial* _dial = nullptr;
    SignalScreen* _screen = nullptr;

    void initGame();

    void reset();
    void gameOver();

    // Copy the game's state onto the dial and the screen.
    void syncViews(float dt) const;
};
