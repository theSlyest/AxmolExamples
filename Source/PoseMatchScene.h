#pragma once

#include "2d/Scene.h"
#include "2d/Label.h"
#include "2d/Layer.h"
#include "PoseCharacter.h"

class PoseMatchScene : public ax::Scene
{
public:
    CREATE_FUNC(PoseMatchScene);

    bool init() override;

private:
    const float GROUND_Y = 160.0f;
    const float GROUND_HEIGHT = 24.0f;
    const float UI_OFFSET = 24.0f;
    const float FRAME_WIDTH = 3.0f;
    const int START_TIME = 45;
    const int START_POSE = 1;
    const std::string TIMER_KEY = "UpdateTimer";
    const ax::Size BANNER_SIZE = ax::Size(192.0f, 64.0f);
    const ax::Color4B BANNER_COLOR = ax::Color4B(0xff, 0xc2, 0x4a, 255);

    int _score = 0;
    int _time = 0;
    int _pose = 0;

    ax::Label* _scoreLbl = nullptr;
    ax::Label* _endScoreLbl = nullptr;
    ax::Label* _timeLbl = nullptr;
    ax::Label* _poseLbl = nullptr;
    ax::Label* _promptLbl = nullptr;
    ax::LayerColor* _endLayer = nullptr;

    PoseCharacter* _character = nullptr;

    void initEndLayer(const ax::Size& size);

    void reset();
    void gameOver();

    void setScore(int score);
    void setTime(int time);

    void updateTime();

    void nextPose(int exclude);

    void setPose(int pose);

    void onKeyPressed(ax::EventKeyboard::KeyCode key);
};
