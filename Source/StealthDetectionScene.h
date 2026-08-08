#pragma once

#include "2d/Scene.h"
#include "2d/DrawNode.h"
#include "2d/Label.h"
#include "physics/PhysicsContact.h"

enum PhysicsBitmask
{
    PLAYER = 0b0001,
    WALL = 0b0010,
    GUARD_VISION = 0b100,
    DESTINATION = 0b1000
};

class StealthDetectionScene : public ax::Scene
{
public:
    bool init() override;
    
    // implement the "static create()" method manually
    CREATE_FUNC(StealthDetectionScene);

    void update(float delta) override;

private:
    const ax::Vec2 CHAR_SIZE = ax::Vec2(48.0f, 48.0f);
    const ax::Vec2 DEST_SIZE = ax::Vec2(120.0f, 120.0f);
    const ax::Vec2 WALL_SIZE = ax::Vec2(40.0f, 40.0f);
    const ax::Vec2 RESET_SIZE = ax::Vec2(120.0f, 40.0f);
    const ax::Vec2 FLOOR_POS = ax::Vec2(40.0f, 80.0f);
    const ax::Vec2 START_POS = ax::Vec2(140.0f, 360.0f);
    const ax::Vec2 GUARD_POS = ax::Vec2(640.0f, 360.0f);
    const ax::Vec2 DEST_POS = ax::Vec2(1080.0f, 360.0f);
    const ax::Vec2 RESET_POS = ax::Vec2(1020.0f, 20.0f);
    const float GUARD_VISION_RANGE = 300.0f;
    const float GUARD_VISION_ANGLE = 60.0f;
    const float PLAYER_SPEED = 200.0f;

    ax::Vec2 _playerVelocity = ax::Vec2::ZERO;
    ax::PhysicsBody* _playerPhysicsBody = nullptr;
    ax::DrawNode* _player = nullptr;
    ax::DrawNode* _guardVision = nullptr;
    ax::DrawNode* _destination = nullptr;
    ax::Label* _successLbl = nullptr;

    void initPlayer(const ax::Vec2& origin);
    void initGuard(const ax::Vec2& origin);
    void initDestination(const ax::Vec2& origin);
    void initWalls(const ax::Vec2& origin);
    void initResetButton(const ax::Vec2& origin);

    void reset();

    void onKeyPressed(ax::EventKeyboard::KeyCode keyCode);
    void onKeyReleased(ax::EventKeyboard::KeyCode keyCode);

    bool onContactBegin(const ax::PhysicsContact &contact);
    void resetPlayer();
    bool onContactPresolve(const ax::PhysicsContact& contact) const;
    
    ax::DrawNode* createWall(int cols, int rows) const;
};
