#include "BomberGame.h"

#include <algorithm>
#include <cmath>

#include "RandomWalkBrain.h"

USING_NS_AX;

BomberGame::BomberGame(std::unique_ptr<EnemyBrain> brain)
    : _enemies(ENEMY_COUNT), _brain(brain ? std::move(brain) : std::make_unique<RandomWalkBrain>())
{
    _grid.onBombReleased = [this](const int owner) {
        if (owner == PLAYER_OWNER)
            _playerHasBomb = false;
    };
}

void BomberGame::reset()
{
    _grid.reset();
    respawnPlayer();
    _player.setInvincible(false);
    _playerHasBomb = false;
    resetEnemies();
    setLives(START_LIVES);
    setTime(START_TIME);
    _elapsed = 0.0f;
    setScore(0);
    _over = false;
}

void BomberGame::update(const float dt, const Vec2& keyDirection)
{
    if (_over)
        return;

    updateTime(dt);
    _grid.update(dt);
    updatePlayer(dt, keyDirection);
    for (auto& enemy : _enemies)
        updateEnemy(dt, enemy);
}

void BomberGame::dropBomb()
{
    if (!_over)
        tryBomb();
}

void BomberGame::setScore(const int score)
{
    _score = score;
    if (onScoreChanged)
        onScoreChanged(score);
}

void BomberGame::setTime(const int time)
{
    _time = time;
    if (onTimeChanged)
        onTimeChanged(time);
}

void BomberGame::setLives(const int lives)
{
    _lives = lives;
    if (onLivesChanged)
        onLivesChanged(lives);
}

void BomberGame::endGame()
{
    _over = true;
    if (onGameOver)
        onGameOver();
}

void BomberGame::updatePlayer(const float dt, const Vec2& keyDirection)
{
    _player.update(dt);

    if (const Vec2 direction = _player.getDirection(); direction != Vec2::ZERO) {
        const Vec2 nextPos = _player.getPosition() + direction * _player.getSpeed() * dt;
        if (_player.hasReachedDestination(nextPos)) {
            const Vec2 dest = _player.getDestination();
            if (direction == keyDirection) {
                const Vec2 nextDest = GridModel::neighborPosition(dest, direction);
                if (_grid.isSafe(nextDest)) {
                    _player.setPosition(nextPos);
                    _player.setDestination(nextDest);
                }
                else {
                    _player.setPosition(dest);
                    _player.setDirection(Vec2::ZERO);
                }
            }
            else {
                _player.setPosition(dest);
                const Vec2 nextDest = GridModel::neighborPosition(dest, keyDirection);
                if (keyDirection == Vec2::ZERO || !_grid.isSafe(nextDest))
                    _player.setDirection(Vec2::ZERO);
                else {
                    _player.setDestination(nextDest);
                    _player.setDirection(keyDirection);
                }
            }
        }
        else {
            _player.setPosition(nextPos);
        }
    }

    if (checkBurnt(_player) || checkEnemyContact())
        loseLife();
}

void BomberGame::movePlayer(const Vec2& direction)
{
    const Vec2 oldDirection = _player.getDirection();
    if (oldDirection == Vec2::ZERO) {
        const Vec2 dest = GridModel::neighborPosition(_player.getPosition(), direction);
        if (_grid.isSafe(dest)) {
            _player.setDestination(dest);
            _player.setDirection(direction);
        }
    }
    else if (oldDirection + direction == Vec2::ZERO) {
        _player.setDestination(GridModel::neighborPosition(_player.getDestination(), direction));
        _player.setDirection(direction);
    }
}

void BomberGame::updateEnemy(const float dt, Enemy& enemy)
{
    if (!enemy.alive)
        return;

    ActorModel& actor = enemy.actor;
    const Vec2 direction = actor.getDirection();

    if (direction != Vec2::ZERO) {
        const Vec2 nextPos = actor.getPosition() + direction * actor.getSpeed() * dt;
        if (actor.hasReachedDestination(nextPos)) {
            const Vec2 dest = actor.getDestination();
            const Vec2 nextDirection = _brain->chooseDirection(_grid, dest);
            if (direction == nextDirection) {
                const Vec2 nextDest = GridModel::neighborPosition(dest, direction);
                if (_grid.isSafe(nextDest)) {
                    actor.setPosition(nextPos);
                    actor.setDestination(nextDest);
                }
                else {
                    actor.setPosition(dest);
                    actor.setDirection(Vec2::ZERO);
                }
            }
            else {
                actor.setPosition(dest);
                const Vec2 nextDest = GridModel::neighborPosition(dest, nextDirection);
                if (nextDirection == Vec2::ZERO || !_grid.isSafe(nextDest))
                    actor.setDirection(Vec2::ZERO);
                else {
                    actor.setDestination(nextDest);
                    actor.setDirection(nextDirection);
                }
            }
        }
        else
            actor.setPosition(nextPos);
    }
    else {
        const Vec2 nextDirection = _brain->chooseDirection(_grid, actor.getPosition());
        actor.setDestination(GridModel::neighborPosition(actor.getPosition(), nextDirection));
        actor.setDirection(nextDirection);
    }

    if (checkBurnt(actor)) {
        setScore(_score + REWARD);
        enemy.alive = false;

        if (std::ranges::none_of(_enemies, [](const Enemy& e) { return e.alive; }))
            endGame();
    }
}

void BomberGame::updateTime(const float dt)
{
    _elapsed += dt;
    if (_elapsed >= 1.0f) {
        setTime(_time - 1);

        if (_time == 0)
            endGame();

        _elapsed -= 1.0f;
    }
}

void BomberGame::tryBomb()
{
    if (_playerHasBomb)
        return;

    const auto [col, row] = GridModel::centerCoords(_player.getPosition());
    if (_grid.getState(col, row) != TileState::SAFE)
        return;

    if (_grid.isOnFire(col, row))
        _grid.detonate(col, row);
    else {
        _grid.setBomb(col, row, PLAYER_OWNER);
        _playerHasBomb = true;
    }
}

bool BomberGame::checkBurnt(const ActorModel& actor) const
{
    if (actor.isInvincible())
        return false;

    const Vec2 position = actor.getPosition();
    const auto first = GridModel::positionToCoords(position + Vec2(ActorModel::OFFSET, ActorModel::OFFSET));
    if (_grid.isOnFire(first.first, first.second))
        return true;

    const auto second = GridModel::positionToCoords(
        position + Vec2(GridModel::TILE_SIZE - ActorModel::OFFSET, GridModel::TILE_SIZE - ActorModel::OFFSET));
    return second != first && _grid.isOnFire(second.first, second.second);
}

bool BomberGame::checkEnemyContact() const
{
    if (_player.isInvincible())
        return false;

    const Vec2 pos = _player.getPosition();
    for (const auto& enemy : _enemies) {
        if (!enemy.alive)
            continue;

        const Vec2 delta = enemy.actor.getPosition() - pos;
        if (std::abs(delta.x) < ActorModel::SIZE && std::abs(delta.y) < ActorModel::SIZE)
            return true;
    }

    return false;
}

void BomberGame::respawnPlayer()
{
    _player.setPosition(GridModel::coordsToPosition({ 1, 1 }));
    _player.setDirection(Vec2::ZERO);
    _player.setDestination(_player.getPosition());
}

void BomberGame::loseLife()
{
    setLives(_lives - 1);
    if (_lives == 0)
        endGame();
    else {
        respawnPlayer();
        _player.setInvincible(true);
    }
}

void BomberGame::resetEnemies()
{
    static const GridCoords SPAWNS[ENEMY_COUNT] = { { 1, 9 }, { 11, 1 }, { 11, 9 } };

    for (size_t i = 0; i < _enemies.size(); ++i) {
        _enemies[i].actor.setPosition(GridModel::coordsToPosition(SPAWNS[i]));
        _enemies[i].actor.setDirection(Vec2::ZERO);
        _enemies[i].alive = true;
    }
}
