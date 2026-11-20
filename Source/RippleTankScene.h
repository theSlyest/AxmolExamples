#pragma once

#include <memory>
#include <vector>

#include "2d/Scene.h"
#include "Contact.h"
#include "RippleControls.h"
#include "RippleHud.h"
#include "RoundOverLayer.h"
#include "StartPrompt.h"
#include "TankGame.h"
#include "Water.h"

class RippleTankScene : public ax::Scene
{
public:
    bool init() override;
    
    // implement the "static create()" method manually
    CREATE_FUNC(RippleTankScene);

    void update(float dt) override;

private:
    std::unique_ptr<TankGame> _game;

    RippleHud* _hud = nullptr;
    Water* _water = nullptr;
    std::vector<Contact*> _contacts;
    StartPrompt* _startPrompt = nullptr;
    RoundOverLayer* _roundOver = nullptr;

    RippleControls* _controls = nullptr;

    void initHud(const ax::Vec2& origin, const ax::Size& visibleSize);
    void initWater(const ax::Size& visibleSize);
    void initContacts();
    void initStartPrompt(const ax::Size& visibleSize);
    void initRoundOver();
    void initGame();

    void initControls();

    // A touch starts the round and, on the water, sends a ripple. Returns true if it was on the water.
    bool onTouch(ax::Touch* touch) const;

    // Show what the start, the play and the end of a round look like.
    void onStateChanged(TankGame::State state) const;

    // Move the contacts' nodes to where the game has them.
    void syncContacts() const;
};
