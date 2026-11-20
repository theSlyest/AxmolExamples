#pragma once

#include <vector>

#include "2d/Label.h"
#include "2d/Layer.h"
#include "2d/Scene.h"
#include "EventListenerKeyboard.h"
#include "EventListenerTouch.h"

#include "Contact.h"
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

    Water* _water = nullptr;
    std::vector<Contact*> _contacts;

    int _score = 0;
    int _timer = 0;
    bool _isStatePlay = false;

    ax::Size _waterSize;

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

    void initTouchListener();
    void initKeyboardListener();

    void setScore(int score);
    void setTimer(int timer);

    bool onTouchBegan(ax::Touch* touch, ax::Event* event);
    void onKeyPressed(ax::EventKeyboard::KeyCode key, ax::Event* event);

    void setStateStart();
    void setStatePlay();
    void setStateEnd();

    ax::Vec2 randomWaterLocation() const;
    void checkContactReached(ax::Vec2 touchLocation);
    void respawnContact(Contact* contact, ax::Vec2 touchLocation);
    bool isWithinReach(ax::Vec2 posA, ax::Vec2 posB);

    void updateContact(float dt, Contact* contact) const;
    void updateTimer();
};

