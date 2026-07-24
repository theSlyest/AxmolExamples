#include "BombermanScene.h"

USING_NS_AX;

// on "init" you need to initialize your instance
bool BombermanScene::init()
{
    if ( !Scene::init() )
    {
        return false;
    }

    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    initScore(origin, visibleSize);
    initTime(origin, visibleSize);
    initLives(origin, visibleSize);
    initGrid(origin);
    initEndLayer();
    initKeyboardListener();

    reset();

    return true;
}

void BombermanScene::update(float dt)
{
    updateTime(dt);
    _grid->update(dt);
    updatePlayer(dt);
    for (int i = 0; i < 3; ++i)
        updateEnemy(dt, _enemies[i]);
}

void BombermanScene::reset()
{
    _grid->reset();
    _player->setPosition(_grid->getTilePosition(1, 1));
    resetEnemies();
    setLives(START_LIVES);
    setTime(START_TIME);
    _elapsed = 0.0f;
    setScore(0);
    _endLayer->setVisible(false);
    scheduleUpdate();
}

void BombermanScene::initScore(const Vec2 &origin, const Size &size)
{
    _score = 0;
    _scoreLbl = Label::createWithTTF("Score: 0", "fonts/arial.ttf", UI_FONT_SIZE);
    _scoreLbl->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
    _scoreLbl->setPosition(origin.x + UI_OFFSET, origin.y + size.height - UI_OFFSET);
    addChild(_scoreLbl);
}

void BombermanScene::initTime(const Vec2 &origin, const Size &size)
{
    _time = 0;
    _timeLbl = Label::createWithTTF("Time: 0", "fonts/arial.ttf", UI_FONT_SIZE);
    _timeLbl->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    _timeLbl->setPosition(origin.x + size.width / 2.0f, origin.y + size.height - UI_OFFSET);
    addChild(_timeLbl);
}

void BombermanScene::initLives(const Vec2 &origin, const Size &size)
{
    _lives = 0;
    _livesLbl = Label::createWithTTF("Lives: 0", "fonts/arial.ttf", UI_FONT_SIZE);
    _livesLbl->setAnchorPoint(Vec2::ANCHOR_TOP_RIGHT);
    _livesLbl->setPosition(origin.x + size.width - UI_OFFSET, origin.y + size.height - UI_OFFSET);
    addChild(_livesLbl);
}

void BombermanScene::initGrid(const Vec2 &origin)
{
    _grid = BomberGrid::create();
    _grid->setPosition(origin + GRID_OFFSET);
    addChild(_grid);
    initEnemies();
    initPlayer();
}

void BombermanScene::initEndLayer()
{
    _endLayer = LayerColor::create(Color4B(0, 0, 0, 128));
    _endLayer->setVisible(false);
    addChild(_endLayer);

    Size size = _endLayer->getContentSize();
    auto gameOver = Label::createWithTTF("GAME OVER", "fonts/arial.ttf", GAME_OVER_SIZE);
    gameOver->setPosition(size.width / 2.0f, size.height / 2.0f);
    _endLayer->addChild(gameOver);

    _endScoreLbl = Label::createWithTTF("Score: 0", "fonts/arial.ttf", UI_FONT_SIZE);
    _endScoreLbl->setPosition(size.width / 2.0f, size.height / 2.0f - 36.0f);
    _endLayer->addChild(_endScoreLbl);

    auto restart = Label::createWithTTF("Press R to Restart", "fonts/arial.ttf", HELP_SIZE);
    restart->setPosition(size.width / 2.0f, size.height / 2.0f - 72.0f);
    _endLayer->addChild(restart);
}

void BombermanScene::initPlayer()
{
    _player = BomberPlayer::create();
    _grid->addChild(_player);
}

void BombermanScene::initEnemies()
{
    for (int i = 0; i < 3; ++i) {
        auto enemy = BomberPlayer::create(true);
        _grid->addChild(enemy);
        _enemies.push_back(enemy);
    }
}

void BombermanScene::initKeyboardListener()
{
    _keyboardListener = EventListenerKeyboard::create();
    _keyboardListener->onKeyPressed = [this](EventKeyboard::KeyCode key, Event *) { onKeyPressed(key); };
    _keyboardListener->onKeyReleased = [this](EventKeyboard::KeyCode key, Event *) { onKeyReleased(key); };
    _eventDispatcher->addEventListenerWithSceneGraphPriority(_keyboardListener, this);

    _keyDirection = Vec2::ZERO;
}

void BombermanScene::setScore(int score)
{
    _score = score;
    _scoreLbl->setString("Score: " + std::to_string(score));
}

void BombermanScene::setTime(int time)
{
    _time = time;
    _timeLbl->setString("Time: " + std::to_string(time));
}

void BombermanScene::setLives(int lives)
{
    _lives = lives;
    _livesLbl->setString("Lives: " + std::to_string(lives));
}

void BombermanScene::onKeyPressed(EventKeyboard::KeyCode key)
{
    switch (key) {
        case EventKeyboard::KeyCode::KEY_LEFT_ARROW:
        case EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
        case EventKeyboard::KeyCode::KEY_UP_ARROW:
        case EventKeyboard::KeyCode::KEY_DOWN_ARROW:
            if (_endLayer->isVisible())
                return;

            if (_keyDirection != Vec2::ZERO)
                return;
    
            _keyDirection = DIRECTIONS.at(key);
            movePlayer(_keyDirection);
            break;

        case EventKeyboard::KeyCode::KEY_SPACE:
            if (_endLayer->isVisible())
                return;

            tryBomb(_player);
            break;

        case EventKeyboard::KeyCode::KEY_R:
            reset();
            break;
    }
}

void BombermanScene::onKeyReleased(EventKeyboard::KeyCode key)
{
    switch (key) {
        case EventKeyboard::KeyCode::KEY_LEFT_ARROW:
        case EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
        case EventKeyboard::KeyCode::KEY_UP_ARROW:
        case EventKeyboard::KeyCode::KEY_DOWN_ARROW:
            if (_endLayer->isVisible())
                return;

            if (_keyDirection != DIRECTIONS.at(key))
                return;
            
            _keyDirection = Vec2::ZERO;
            break;
    }
}

void BombermanScene::updatePlayer(float dt)
{
    _player->update(dt);
    const Vec2& direction = _player->getDirection();

    if (direction != Vec2::ZERO) {
        Vec2 nextPos = _player->getPosition() + direction * _player->getSpeed() * dt;
        if (_player->hasReachedDestination(nextPos)) {
            Vec2 dest = _player->getDestination();
            if (direction == _keyDirection) {
                const Vec2 nextDest = dest + direction * GridTile::SIZE;
                if (_grid->getTile(nextDest)->getState() == TileState::SAFE) {
                    _player->setPosition(nextPos);
                    _player->setDestination(nextDest);
                }
                else {
                    _player->setPosition(dest);
                    _player->setDirection(Vec2::ZERO);
                }
            }
            else {
                _player->setPosition(dest);
                const Vec2 nextDest = dest + _keyDirection * GridTile::SIZE;
                if (_keyDirection == Vec2::ZERO || _grid->getTile(nextDest)->getState() != TileState::SAFE)
                    _player->setDirection(Vec2::ZERO);
                else {
                    _player->setDestination(nextDest);
                    _player->setDirection(_keyDirection);
                }
            }
        }
        else {
            _player->setPosition(nextPos);
        }
    }

    if (checkBurnt(_player))
        loseLife();
}

void BombermanScene::movePlayer(const ax::Vec2 &direction)
{
    const Vec2& oldDirection = _player->getDirection();
    if (oldDirection == Vec2::ZERO) {
        const Vec2 dest = _player->getPosition() + direction * GridTile::SIZE;
        if (_grid->getTile(dest)->getState() == TileState::SAFE) {
            _player->setDestination(dest);
            _player->setDirection(direction);
        }
    }
    else if (oldDirection + direction == Vec2::ZERO) {
        _player->setDestination(_player->getDestination() + direction * GridTile::SIZE);
        _player->setDirection(direction);
    }
}

void BombermanScene::updateEnemy(float dt, BomberPlayer *enemy)
{
    if (!enemy->isVisible())
        return;

    const Vec2& direction = enemy->getDirection();

    if (direction != Vec2::ZERO) {
        Vec2 nextPos = enemy->getPosition() + direction * enemy->getSpeed() * dt;
        if (enemy->hasReachedDestination(nextPos)) {
            Vec2 nextDirection = chooseDirection(enemy);
            Vec2 dest = enemy->getDestination();
            if (direction == nextDirection) {
                const Vec2 nextDest = dest + direction * GridTile::SIZE;
                if (_grid->getTile(nextDest)->getState() == TileState::SAFE) {
                    enemy->setPosition(nextPos);
                    enemy->setDestination(nextDest);
                }
                else {
                    enemy->setPosition(dest);
                    enemy->setDirection(Vec2::ZERO);
                }
            }
            else {
                enemy->setPosition(dest);
                const Vec2 nextDest = dest + nextDirection * GridTile::SIZE;
                if (nextDirection == Vec2::ZERO || _grid->getTile(nextDest)->getState() != TileState::SAFE)
                    enemy->setDirection(Vec2::ZERO);
                else {
                    enemy->setDestination(nextDest);
                    enemy->setDirection(nextDirection);
                }
            }
        }
        else
            enemy->setPosition(nextPos);
    }
    else {
        const Vec2& nextDirection = chooseDirection(enemy);
        enemy->setDestination(enemy->getPosition() + nextDirection * GridTile::SIZE);
        enemy->setDirection(nextDirection);
    }

    if (checkBurnt(enemy)) {
        setScore(_score + REWARD);
        enemy->setVisible(false);

        bool won = true;
        for (auto e : _enemies) {
            if (e->isVisible()) {
                won = false;
                break;
            }
        }

        if (won)
            showGameOver();
    }
}

ax::Vec2 BombermanScene::chooseDirection(BomberPlayer *enemy)
{
    std::vector<Vec2> dirs;
    Vec2 pos = enemy->getPosition();
    for (const auto& dir : DIRECTIONS) {
        const Vec2 next = pos + dir.second * GridTile::SIZE;
        auto tile = _grid->getTile(next);
        if (tile->getState() == TileState::SAFE/* && !tile->isOnFire()*/)
            dirs.push_back(dir.second);
    }

    if (dirs.empty())
        return Vec2::ZERO;

    return dirs[random(0ul, dirs.size() - 1)];
}
void BombermanScene::updateTime(float dt)
{
    _elapsed += dt;
    if (_elapsed >= 1.0f) {
        setTime(_time - 1);

        if (_time == 0.0f)
            showGameOver();

        _elapsed = 1.0f - _elapsed;
    }
}

void BombermanScene::tryBomb(BomberPlayer* bomber)
{
    if (bomber->hasBomb())
        return;
    
    auto tile = _grid->getTile(bomber->getPosition() + Vec2(GridTile::SIZE, GridTile::SIZE) / 2.0f);
    if (tile->getState() != TileState::SAFE)
        return;

    if (tile->isOnFire())
        _grid->detonate(tile);
    else {
        auto bomb = BomberBomb::create(bomber);
        bomb->setPosition(tile->getPosition());
        _grid->addChild(bomb);
        tile->setBomb(bomb);
        bomber->dropBomb();
    }
}

bool BombermanScene::checkBurnt(BomberPlayer *bomber)
{
    if (bomber->isInvincible())
        return false;

    auto tile1 = _grid->getTile(bomber->getPosition() + Vec2(BomberPlayer::OFFSET, BomberPlayer::OFFSET));
    if (tile1->isOnFire())
        return true;
    
    auto tile2 = _grid->getTile(bomber->getPosition() + Vec2(GridTile::SIZE - BomberPlayer::OFFSET, GridTile::SIZE -BomberPlayer::OFFSET));
    if (tile2 != tile1 && tile2->isOnFire())
        return true;

    return false;
}

void BombermanScene::showGameOver()
{
    unscheduleUpdate();
    _endScoreLbl->setString(_scoreLbl->getString());
    _endLayer->setVisible(true);
}

void BombermanScene::loseLife()
{
    setLives(_lives - 1);
    if (_lives == 0)
        showGameOver();
    else {
        _player->setPosition(_grid->getTilePosition(1, 1));
        _player->setInvincible(true);
    }
}

void BombermanScene::resetEnemies()
{
    _enemies[0]->setPosition(_grid->getTilePosition(1, 9));
    _enemies[1]->setPosition(_grid->getTilePosition(11, 1));
    _enemies[2]->setPosition(_grid->getTilePosition(11, 9));
    for (auto& enemy : _enemies) {
        enemy->setDirection(Vec2::ZERO);
        enemy->setVisible(true);
    }
}
