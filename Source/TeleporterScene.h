#pragma once

#include "axmol.h"

enum BODY_BITMASK { PLAYER = 1, BALL = 2, BLOCK = 4 };

class TeleporterScene : public ax::Scene
{
public:
    bool init() override;
    
    // implement the "static create()" method manually
    CREATE_FUNC(TeleporterScene);

private:
    static const ax::Color4B BLOCK_COLOR;

    static constexpr float PLAYER_SIZE = 50.0f;
    static constexpr float BALL_SCALE = 0.5f;
    static constexpr float DOUBLE_TAP_INTERVAL = 0.15f;
    static constexpr float HOLD_DURATION = 0.3f;

    const std::string KEY_SINGLE_TAP = "keySingleTap";
    const std::string KEY_HOLD = "keyHold";
    const std::string KEY_THROW_BALL = "keyThrowBall";
    const std::string KEY_HIDE_BALL = "keyHideBall";
    const std::string KEY_TELEPORT = "keyTeleport";
    const ax::Vec2 GRAVITY = ax::Vec2(0.0f, -98.0f);

    bool _isHold = false;
    bool _isSecondTap = false;

    ax::Sprite* _ball = nullptr;
    ax::Sprite* _player = nullptr;

    static ax::Node* createBlock(const ax::Vec2& size);

    void initLevel(const ax::Vec2& origin, const ax::Vec2& size);
    void initBall();
    void initPlayer(const ax::Vec2& origin);

    void onKeyPressed(ax::EventKeyboard::KeyCode key);

    bool onTouchBegan();
    void onTouchEnded(const ax::Touch* touch);

    bool onContactBegin();

    void hideBall() const;

    void throwBall(const ax::Vec2& position, float speed, const ax::Vec2& gravity);
};
