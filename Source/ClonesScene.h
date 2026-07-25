/****************************************************************************
 Copyright (c) 2017-2018 Xiamen Yaji Software Co., Ltd.
 
 http://www.cocos2d-x.org
 
 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 ****************************************************************************/

#pragma once

#include "axmol.h"
#include "InputRecorder.h"
#include "Player.h"

enum class XMove { None, Left, Right };

class ClonesScene : public ax::Scene
{
public:
    bool init() override;

    void update(float dt) override;
    
    // implement the "static create()" method manually
    CREATE_FUNC(ClonesScene);

private:
    const float REC_DURATION = 30.0f;
    const std::string REC_KEY = "inputRecording";

    Player* _player = nullptr;
    Player* _currentClone = nullptr;
    ax::Sprite* _goal = nullptr;
    ax::Sprite* _checkbox = nullptr;
    ax::Label* _goalReachedLabel = nullptr;
    ax::Label* _timerLabel = nullptr;

    ax::Layer* _gameOverLayer = nullptr;

    ax::EventListenerKeyboard* _keyboardListener = nullptr;
    ax::EventListenerPhysicsContact* _contactListener = nullptr;
    std::vector<Player*> _clones;

    int _goalReachedCount = 0;
    int _timer = 0;
    bool _isPlaying = false;

    XMove _xMove = XMove::None;
    bool _isJumping = false;

    InputRecorder _recorder;

    void initFloor(const ax::Size &visibleSize, const ax::Vec2 &origin);
    void initGameOverLayer(const ax::Size &visibleSize, const ax::Vec2 &origin);

    void resetGame();

    bool onKeyPressed(ax::EventKeyboard::KeyCode key);
    bool onKeyReleased(ax::EventKeyboard::KeyCode key);

    bool onContactBegin(const ax::PhysicsContact&);

    void scheduleReplay();
    void unscheduleReplay();

    void endGame();

    void replayInput(ax::EventKeyboard::KeyCode keyCode, bool isPressed);

    void updateGoalReachedLabel();
    void updateTimerLabel();

    void applyInitialPlayerPosition(Player* player);
};
