#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "axmol.h"
#include "Ball.h"
#include "Brick.h"
#include "Paddle.h"

class HelloWorld : public ax::Scene
{
public:
    virtual bool init();

    virtual void update(float dt) override;

private:
    int _score;
    int _lives;
    bool _isResting;
    float _lowerY;

    ax::Label* _scoreLabel;
    ax::Label* _livesLabel;
    ax::Label* _endLabel;
    ax::EventListenerKeyboard* _keyListener;
    ax::Vec2 _paddleStart;
    ax::Vec2 _ballStart;

    Paddle* _paddle;
    Ball* _ball;
    std::list<Brick*> _bricks;

    void initScore(const ax::Size& visibleSize, const ax::Vec2& origin);
    void initLives(const ax::Size& visibleSize, const ax::Vec2& origin);
    void initPaddle(const ax::Size& visibleSize, const ax::Vec2& origin);
    void initBricks();
    void initBall(const ax::Size& visibleSize, const ax::Vec2& origin);
    void initKeyboardListener();
    void initEndLabel(const ax::Size& visibleSize, const ax::Vec2& origin);

    void setScore(int value);
    void setLives(int value);

    void onKeyPressed(ax::EventKeyboard::KeyCode key);
    void onKeyReleased(ax::EventKeyboard::KeyCode key);

    void softReset();
    void hardReset();
    void clearBricks();
    
    void checkBrickCollision();
    void checkPaddleCollision();
    void showResult(bool win);
};

#endif // __HELLOWORLD_SCENE_H__
