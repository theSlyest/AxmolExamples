#pragma once

#include <memory>
#include <vector>

#include "2d/Scene.h"
#include "EventListenerKeyboard.h"
#include "EventListenerTouch.h"

#include "Contact.h"
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

    ax::EventListenerTouchOneByOne* _touchListener = nullptr;
    ax::EventListenerKeyboard* _keyboardListener = nullptr;

    void initHud(const ax::Vec2& origin, const ax::Size& visibleSize);
    void initWater(const ax::Size& visibleSize);
    void initContacts();
    void initStartPrompt(const ax::Size& visibleSize);
    void initRoundOver();
    void initGame();

    void initTouchListener();
    void initKeyboardListener();

    bool onTouchBegan(ax::Touch* touch, ax::Event* event);
    void onKeyPressed(ax::EventKeyboard::KeyCode key, ax::Event* event) const;

    // Show what the start, the play and the end of a round look like.
    void onStateChanged(TankGame::State state) const;

    // Move the contacts' nodes to where the game has them.
    void syncContacts() const;
};
