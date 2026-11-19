#pragma once

#include <functional>
#include <memory>
#include <vector>

#include "ActorModel.h"
#include "EnemyBrain.h"
#include "GridModel.h"

// The rules of the game: movement, bombs, fire, lives, score and time.
//
// This class has no nodes and no drawing, so it can be read and tested on its
// own. It owns the grid and the actors' state; the scene draws them and
// listens to the callbacks to update the HUD.
class BomberGame
{
public:
    static constexpr int START_LIVES = 3;
    static constexpr int START_TIME = 120;
    static constexpr int REWARD = 100;
    static constexpr float PLAYER_SPEED = 200.0f;
    static constexpr float ENEMY_SPEED = 100.0f;
    static constexpr int ENEMY_COUNT = 3;

    struct Enemy
    {
        ActorModel actor{ ENEMY_SPEED };
        bool alive = true;
    };

    // `brain` drives the enemies; the default is RandomWalkBrain.
    explicit BomberGame(std::unique_ptr<EnemyBrain> brain = nullptr);

    // The grid reports bomb releases back to this object, so it must not move.
    BomberGame(const BomberGame&) = delete;
    BomberGame& operator=(const BomberGame&) = delete;

    // Notifications for the view.
    std::function<void(int)> onScoreChanged;
    std::function<void(int)> onLivesChanged;
    std::function<void(int)> onTimeChanged;
    std::function<void()> onGameOver;

    void reset();

    // keyDirection is the direction currently held by the player.
    void update(float dt, const ax::Vec2& keyDirection);

    // Player actions.
    void movePlayer(const ax::Vec2& direction);
    void dropBomb();

    [[nodiscard]] bool isOver() const { return _over; }
    [[nodiscard]] int getScore() const { return _score; }
    [[nodiscard]] int getLives() const { return _lives; }
    [[nodiscard]] int getTime() const { return _time; }
    [[nodiscard]] bool playerHasBomb() const { return _playerHasBomb; }

    // The state the views draw. The non-const versions let tests set up a situation.
    GridModel& getGrid() { return _grid; }
    [[nodiscard]] const GridModel& getGrid() const { return _grid; }
    ActorModel& getPlayer() { return _player; }
    [[nodiscard]] const ActorModel& getPlayer() const { return _player; }
    std::vector<Enemy>& getEnemies() { return _enemies; }
    [[nodiscard]] const std::vector<Enemy>& getEnemies() const { return _enemies; }

private:
    static constexpr int PLAYER_OWNER = 0; // owner id of the player's bombs in the grid

    int _score = 0;
    int _time  = 0;
    int _lives = 0;
    float _elapsed = 0;
    bool _over = true;
    bool _playerHasBomb = false;

    GridModel _grid;
    ActorModel _player{ PLAYER_SPEED };
    std::vector<Enemy> _enemies;
    std::unique_ptr<EnemyBrain> _brain;

    void setScore(int score);
    void setTime(int time);
    void setLives(int lives);

    void updatePlayer(float dt, const ax::Vec2& keyDirection);
    void updateEnemy(float dt, Enemy& enemy);
    void updateTime(float dt);

    void tryBomb();
    [[nodiscard]] bool checkBurnt(const ActorModel& actor) const;
    [[nodiscard]] bool checkEnemyContact() const;

    void respawnPlayer();
    void resetEnemies();
    void loseLife();
    void endGame();
};
