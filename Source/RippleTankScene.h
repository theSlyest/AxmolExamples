#pragma once

#include <memory>
#include <vector>

#include "2d/Label.h"
#include "2d/Layer.h"
#include "2d/Scene.h"
#include "EventListenerKeyboard.h"
#include "EventListenerTouch.h"

#include "Contact.h"
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
    static const ax::Color4B TextColor;

    std::unique_ptr<TankGame> _game;

    Water* _water = nullptr;
    std::vector<Contact*> _contacts;

    ax::Label* _scoreLabel = nullptr;
    ax::Label* _endScoreLabel = nullptr;
    ax::Label* _timerLabel = nullptr;

    ax::LayerColor* _startLayer = nullptr;
    ax::LayerColor* _endLayer = nullptr;

    ax::EventListenerTouchOneByOne* _touchListener = nullptr;
    ax::EventListenerKeyboard* _keyboardListener = nullptr;

    void initScoreLabel(const ax::Vec2& origin, const ax::Size& visibleSize);
    void initTimerLabel(const ax::Vec2& origin, const ax::Size& visibleSize);
    void initWater(const ax::Size& visibleSize);
    void initContacts();
    void initStartLayer(const ax::Size& visibleSize);
    void initEndLayer(const ax::Size& visibleSize);
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
