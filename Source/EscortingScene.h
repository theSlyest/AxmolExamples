#pragma once

#include "axmol.h"
#include "Destination.h"
#include "Npc.h"
#include "MoveRecorder.h"
#include "Player.h"

class EscortingScene : public ax::Scene
{
public:
    enum class GameState { IDLE, ESCORTING, FOLLOWING, COMPLETE };

    bool init() override;
    
    // implement the "static create()" method manually
    CREATE_FUNC(EscortingScene);

    void update(float dt) override;

    void updateEscortPosition(float dt);

private:
    static constexpr float OFFSET = 8.0f;
    static constexpr float TALK_DISTANCE = 20.0f;

    GameState _state = GameState::IDLE;

    ax::Size _visibleSize;
    ax::Vec2 _origin;

    Destination* _destination = nullptr;
    Npc* _npc = nullptr;
    Player* _player = nullptr;

    MoveRecorder _recorder;

    const std::map<char, ax::Vec2> Directions {
        { 'W', ax::Vec2( 0.0f,  1.0f) },
        { 'S', ax::Vec2( 0.0f, -1.0f) },
        { 'A', ax::Vec2(-1.0f,  0.0f) },
        { 'D', ax::Vec2( 1.0f,  0.0f) },
    };

    ax::EventListenerKeyboard* _keyboardListener = nullptr;
    ax::Label* _completeLabel = nullptr;

    void initDestination();
    void initEscort();
    void initPlayer();
    void initComplete();

    void initKeyboardListener();

    void onKeyPressed(ax::EventKeyboard::KeyCode key);
    void movePlayer(char key);
    void onKeyReleased(ax::EventKeyboard::KeyCode key);

    void stopPlayer();

    bool tryTalk();
    void escortToDestination();
    float playerEscortDistance();
    void updateCharacterPosition(Character *character, float dt) const;
};
