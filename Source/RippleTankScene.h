#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "axmol.h"
#include "Contact.h"
#include "Water.h"

class RippleTankScene : public ax::Scene
{
public:
    virtual bool init() override;
    
    // implement the "static create()" method manually
    CREATE_FUNC(RippleTankScene);

    virtual void update(float dt) override;

private:
    static const ax::Color4B TextColor;

    Water* _water;
    std::vector<Contact*> _contacts;

    int _score;
    int _timer;
    bool _isStatePlay;

    ax::Size _waterSize;

    ax::Label* _scoreLabel;
    ax::Label* _endScoreLabel;
    ax::Label* _timerLabel;

    ax::LayerColor* _startLayer;
    ax::LayerColor* _endLayer;

    ax::EventListenerTouchOneByOne* _touchListener;
    ax::EventListenerKeyboard* _keyboardListener;

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

    ax::Vec2 randomWaterLocation();
    void checkContactReached(ax::Vec2 touchLocation);
    void respawnContact(Contact* contact, ax::Vec2 touchLocation);
    bool isWithinReach(ax::Vec2 posA, ax::Vec2 posB);

    void updateContact(float dt, Contact* contact);
    void updateTimer();
};

#endif // __HELLOWORLD_SCENE_H__
