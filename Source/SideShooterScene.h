#pragma once

#include "axmol.h"

struct Rotation { 
    float speed;
    explicit Rotation(const float speed) : speed(speed) {}
};

class SideShooterScene : public ax::Scene
{
public:
    // implement the "static create()" method manually
    CREATE_FUNC(SideShooterScene);

    bool init() override;

    void initKeyboardListener();

    void update(float dt) override;

private:
    const float PLAYER_OFFSET = 150.0f;
    const float PLAYER_SPEED = 360.0f;
    const float BULLET_SPEED = 640.0f;
    const float FUEL_RATE = 5.0f;
    const float GAME_OVER_SIZE = 64.0f;
    const float INFO_SIZE = 28.0f;
    const float INFO_OFFSET = 16.0f;
    const float SPAWN_DELAY = 0.8f;
    const float REFUEL_DELAY = 4.0f;
    const float REFUEL_RATE = 20.0f;
    const float OBJECT_SPEED = 320.0f;

    int _bg = 0;
    int _sf = 0;

    float _speed = 0.0f;
    float _bgSpeed = 0.0f;
    float _starSpeed = 0.0f;
    float _playerMaxY = 0.0f;
    float _playerMinY = 0.0f;
    float _objectMaxY = 0.0f;
    float _objectMinY = 0.0f;
    float _objectStartX = 0.0f;
    float _objectEndX = 0.0f;
    float _bulletMaxX = 0.0f;
    float _fuel = 0.0f;
    float _spawnTimer = 0.0f;
    float _refuelTimer = 0.0f;
    float _elapsed = 0.0f;

    bool _spawnRefuel = false;

    const std::string OBJECT_TEX[7] { 
        "spawnable/fuel.png",
        "spawnable/plane1.png",
        "spawnable/plane2.png",
        "spawnable/plane3.png",
        "spawnable/plane4.png",
        "spawnable/rock01.png",
        "spawnable/rock02.png"
    };

    ax::Sprite* _backgrounds[2] { nullptr, nullptr};
    ax::Sprite* _starFields[2] { nullptr, nullptr};
    ax::Sprite* _objects[8] { nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr };
    ax::Sprite* _player = nullptr;
    ax::Sprite* _bullet = nullptr;

    ax::Label* _fuelLbl = nullptr;
    ax::Label* _timerLbl = nullptr;
    ax::Label* _gameOverLbl = nullptr;

    static void preloadSounds();
    static void playSound(const std::string& path);

    void preloadTextures() const;

    void initBackground();
    void initStarField();
    void initPlayer(const ax::Vec2& origin, const ax::Vec2& size);
    void initBullet(const ax::Vec2& origin, const ax::Vec2& size);
    void initFuel(const ax::Vec2& origin);
    void initTimer(const ax::Vec2& origin, const ax::Vec2& size);
    void initGameOver(const ax::Vec2& origin, const ax::Vec2& size);
    void initObjects(const ax::Vec2 &origin, const ax::Vec2 &size);

    void updateBackground(float dt);
    void updateStarField(float dt);
    void updatePlayer(float dt) const;
    void updateBullet(float dt) const;
    void updateFuel(float dt);
    void updateTimer(float dt);
    void updateRefuelTimer(float dt);
    void updateSpawnTimer(float dt);
    void updateObjects(float dt);

    static void resetObject(ax::Sprite* object);

    static bool SpritesDidCollide(ax::Sprite* pSpriteA, ax::Sprite* pSpriteB);
    static bool SpritesCollidedWithAlpha(ax::Sprite* pSpriteA, ax::Sprite* pSpriteB);

    void onKeyPressed(ax::EventKeyboard::KeyCode key);
    void onKeyReleased(ax::EventKeyboard::KeyCode key);

    void shootBullet() const;
    void spawn(int index) const;

    void gameOver();
};
