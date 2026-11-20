#pragma once

#include "2d/Scene.h"
#include "EventListenerKeyboard.h"
#include "SignalDial.h"
#include "SignalScreen.h"

class SignalLockScene : public ax::Scene
{
public:
    bool init() override;
    
    // implement the "static create()" method manually
    CREATE_FUNC(SignalLockScene);

    void update(float dt) override;

private:
    // Rules. Distances are fractions of the dial, between 0 and 1.
    static constexpr float LOCK_DIST = 0.03f;   // how close counts as being on the station
    static constexpr float LOCK_DELAY = 0.6f;   // seconds on the station to lock it
    static constexpr float LOCK_SAFE = 0.25f;   // a new station is at least this far from the dial
    static constexpr float DETECT_DIST = 0.2f;  // the screen starts to clear up inside this distance
    static constexpr int START_TIME = 45;

    // Layout, in pixels.
    static constexpr float LABELS_OFFSET = 20.0f;
    static constexpr float DIAL_BOTTOM = 40.0f;
    static constexpr float HELP_BOTTOM = 240.0f;
    static constexpr float SCREEN_BOTTOM = 288.0f;

    static inline const ax::Color4B UI_COLOR = ax::Color4B(0xff, 0xc2, 0x4a, 255);

    int _score = 0;
    int _time = 0;
    float _timer = 0.0f;
    float _speed = 0.0f;
    float _velocity = 0.0f;
    float _station = 0.0f;
    float _lockTime = 0.0f;

    ax::Label* _scoreLabel = nullptr;
    ax::Label* _endScoreLabel = nullptr;
    ax::Label* _timeLabel = nullptr;
    ax::LayerColor* _endLayer = nullptr;
    ax::EventListenerKeyboard* _keyListener = nullptr;

    SignalDial* _dial = nullptr;
    SignalScreen* _screen = nullptr;

    bool _isPlaying = false;

    void initEndLayer(const ax::Size& size);
    void setScore(int score);
    void setTime(int time);

    void reset();

    void onKeyPressed(ax::EventKeyboard::KeyCode key);
    void onKeyReleased(ax::EventKeyboard::KeyCode key);

    void newStation();

    void startPlaying();
    void gameOver();
};
