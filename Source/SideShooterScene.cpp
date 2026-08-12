#include "SideShooterScene.h"

#include "audio/AudioEngine.h"

USING_NS_AX;

// on "init" you need to initialize your instance
bool SideShooterScene::init()
{
    if (!Scene::init())
        return false;

    preloadSounds();
    preloadTextures();

    const Vec2& size = _director->getVisibleSize();
    const Vec2& origin = _director->getVisibleOrigin();

    initBackground();
    initStarField();
    initPlayer(origin, size);
    initBullet(origin, size);
    initFuel(origin);
    initTimer(origin, size);
    initGameOver(origin, size);
    initObjects(origin, size);

    initKeyboardListener();

    scheduleUpdate();

    return true;
}

void SideShooterScene::initKeyboardListener()
{
    const auto keyListener = EventListenerKeyboard::create();
    keyListener->onKeyPressed = [this](const EventKeyboard::KeyCode key, Event *) { onKeyPressed(key); };
    keyListener->onKeyReleased = [this](const EventKeyboard::KeyCode key, Event *) { onKeyReleased(key); };
    _eventDispatcher->addEventListenerWithSceneGraphPriority(keyListener, this);
}

void SideShooterScene::update(float dt)
{
    updateBackground(dt);
    updateStarField(dt);
    updatePlayer(dt);
    updateBullet(dt);
    updateObjects(dt);
    updateFuel(dt);
    updateTimer(dt);
    updateRefuelTimer(dt);
    updateSpawnTimer(dt);
}

void SideShooterScene::preloadSounds()
{
    AudioEngine::preload("bullet.wav");
    AudioEngine::preload("explosion.wav");
}

void SideShooterScene::playSound(const std::string &path)
{
    AudioEngine::play2d(path);
}

void SideShooterScene::preloadTextures() const
{
    for (int i = 0; i < 6; ++i)
        _director->getTextureCache()->addImage(OBJECT_TEX[i]);
}

void SideShooterScene::initBackground()
{
    _backgrounds[0] = Sprite::create("background.png");
    _backgrounds[0]->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
    addChild(_backgrounds[0], -2);

    _backgrounds[1] = Sprite::create("background.png");
    _backgrounds[1]->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
    _backgrounds[1]->setPositionX(_backgrounds[0]->getBoundingBox().getMaxX() + 1.0f);
    addChild(_backgrounds[1], -2);

    _bg = 0;
    _bgSpeed = OBJECT_SPEED * 0.5f;
}

void SideShooterScene::initStarField()
{
    _starFields[0] = Sprite::create("parallaxStarfield.png");
    _starFields[0]->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
    addChild(_starFields[0], -1);

    _starFields[1] = Sprite::create("parallaxStarfield.png");
    _starFields[1]->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
    _starFields[1]->setPositionX(_starFields[0]->getBoundingBox().getMaxX() + 1.0f);
    addChild(_starFields[1], -1);

    _sf = 0;
    _starSpeed = OBJECT_SPEED * 0.25f;
}

void SideShooterScene::initPlayer(const Vec2& origin, const Vec2& size)
{
    _player = Sprite::create("player.png");
    _player->setName("player.png");
    _player->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    _player->setPosition(origin.x + PLAYER_OFFSET, origin.y + size.height * 0.5f);
    addChild(_player, 1);

    _speed = 0;
    const float half = _player->getContentSize().height * 0.5f;
    _playerMinY = origin.y + half;
    _playerMaxY = origin.y + size.height - half;
}

void SideShooterScene::initBullet(const Vec2 &origin, const Vec2 &size)
{
    _bullet = Sprite::create("spawnable/bullet.png");
    _bullet->setName("spawnable/bullet.png");
    _bullet->setVisible(false);
    addChild(_bullet);

    _bulletMaxX = origin.x + size.width + _bullet->getContentSize().width * 0.5f;
}

void SideShooterScene::initFuel(const Vec2 &origin)
{
    _fuelLbl = Label::createWithTTF("Fuel: 100%", "fonts/retro8bit.ttf", INFO_SIZE);
    _fuelLbl->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
    _fuelLbl->setPosition(origin.x + INFO_OFFSET, origin.y + INFO_OFFSET);
    addChild(_fuelLbl, 2);

    _fuel = 100.0f;
}

void SideShooterScene::initTimer(const Vec2 &origin, const Vec2 &size)
{
    _timerLbl = Label::createWithTTF("00:00", "fonts/retro8bit.ttf", INFO_SIZE);
    _timerLbl->setAnchorPoint(Vec2::ANCHOR_BOTTOM_RIGHT);
    _timerLbl->setPosition(origin.x + size.width - INFO_OFFSET, origin.y + INFO_OFFSET);
    addChild(_timerLbl, 2);

    _elapsed = 0.0f;
}

void SideShooterScene::initGameOver(const Vec2 &origin, const Vec2 &size)
{
    _gameOverLbl = Label::createWithTTF("GAME OVER!", "fonts/retro8bit.ttf", GAME_OVER_SIZE);
    _gameOverLbl->setPosition(origin + size * 0.5f);
    _gameOverLbl->setVisible(false);
    addChild(_gameOverLbl, 2);
}

void SideShooterScene::initObjects(const Vec2 &origin, const Vec2 &size)
{
    for (auto& object : _objects) {
        object = Sprite::create();
        object->setVisible(false);
        object->setUserData(new Rotation(0.0f));
        addChild(object);
    }

    _spawnRefuel = false;
    _refuelTimer = 0.0f;

    _objectMinY = origin.y + 60.0f;
    _objectMaxY = origin.y + size.height - 60.0f;
    _objectStartX = origin.x + size.width + 64.0f;
    _objectEndX = origin.x - 64.0f;
}

void SideShooterScene::updateBackground(const float dt)
{
    for (auto& bg : _backgrounds)
        bg->setPositionX(bg->getPositionX() - dt * _bgSpeed);
    
    if (_backgrounds[_bg]->getBoundingBox().getMaxX() < _director->getVisibleOrigin().x) {
        _backgrounds[_bg]->setPositionX(_backgrounds[1 - _bg]->getBoundingBox().getMaxX() + 1.0f);
        _bg = 1 - _bg;
    }
}

void SideShooterScene::updateStarField(const float dt)
{
    for (auto& sf : _starFields)
        sf->setPositionX(sf->getPositionX() - dt * _starSpeed);
    
    if (_starFields[_sf]->getBoundingBox().getMaxX() < _director->getVisibleOrigin().x) {
        _starFields[_sf]->setPositionX(_starFields[1 - _sf]->getBoundingBox().getMaxX() + 1.0f);
        _sf = 1 - _sf;
    }
}

void SideShooterScene::updatePlayer(const float dt) const
{
    float y = _player->getPositionY() + dt * _speed;
    y = std::max(_playerMinY, std::min(_playerMaxY, y));
    _player->setPositionY(y);
}

void SideShooterScene::updateBullet(const float dt) const
{
    if (!_bullet->isVisible())
        return;

    const float x = _bullet->getPositionX() + dt * BULLET_SPEED;
    if (x >= _bulletMaxX)
        _bullet->setVisible(false);
    else
        _bullet->setPositionX(x);
}


void SideShooterScene::updateFuel(const float dt)
{
    _fuel = std::max(0.0f, _fuel - dt * FUEL_RATE);
    _fuelLbl->setString(StringUtils::format("Fuel: %d%%", static_cast<int>(_fuel)));
    
    if (_fuel == 0.0f)
        gameOver();
}

void SideShooterScene::updateTimer(const float dt)
{
    _elapsed += dt;
    const int timer = static_cast<int>(_elapsed);
    _timerLbl->setString(StringUtils::format("%02d:%02d", timer / 60, timer % 60));
}

void SideShooterScene::updateRefuelTimer(const float dt)
{
    _refuelTimer += dt;
    if (_refuelTimer >= REFUEL_DELAY) {
        _refuelTimer -= REFUEL_DELAY;
        _spawnRefuel = true;
    }
}

void SideShooterScene::updateSpawnTimer(const float dt)
{
    _spawnTimer += dt;
    if (_spawnTimer >= SPAWN_DELAY) {
        _spawnTimer -= SPAWN_DELAY;
        if (_spawnRefuel) {
            _spawnRefuel = false;
            spawn(0);
        }
        else
            spawn(random(1, 6));
    }
}

void SideShooterScene::updateObjects(const float dt)
{
    for (const auto object : _objects) {
        if (!object->isVisible())
            continue;

        if (const float x = object->getPositionX() - dt * OBJECT_SPEED; x > _objectEndX) {
            object->setPositionX(x);
            if (object->getTag() == 0) {
                if (SpritesDidCollide(object, _player))
                    _fuel += dt * REFUEL_RATE;

                continue;
            }
            
            if (SpritesDidCollide(object, _player)) {
                playSound("explosion.wav");
                gameOver();
                
                continue;
            }
            
            if (_bullet->isVisible() && SpritesDidCollide(object, _bullet)) {
                _bullet->setVisible(false);
                resetObject(object);
                playSound("explosion.wav");
                continue;
            }

            if (object->getTag() > 4) 
                object->setRotation(object->getRotation() + dt * static_cast<Rotation*>(object->getUserData())->speed);
        }
        else
            resetObject(object);
    }
}

void SideShooterScene::resetObject(Sprite *object)
{
    object->setVisible(false);
    object->setOpacity(255);
    object->setRotation(0.0f);
    static_cast<Rotation*>(object->getUserData())->speed = 0.0f;
}

bool SideShooterScene::SpritesDidCollide(Sprite *pSpriteA, Sprite *pSpriteB)
{
    if (pSpriteA->getBoundingBox().intersectsRect(pSpriteB->getBoundingBox()))
        return SpritesCollidedWithAlpha(pSpriteA, pSpriteB);

    return false;
}

bool SideShooterScene::SpritesCollidedWithAlpha(Sprite* pSpriteA, Sprite* pSpriteB)
{
    //------------------------------
    // Find the overlapping intersection rectangle in world space
    const Rect rBoundingBoxA = pSpriteA->getBoundingBox();
    const Rect rBoundingBoxB = pSpriteB->getBoundingBox();

    const float fIntersectionLeft   = std::max(rBoundingBoxA.getMinX(), rBoundingBoxB.getMinX());
    const float fIntersectionRight  = std::min(rBoundingBoxA.getMaxX(), rBoundingBoxB.getMaxX());
    const float fIntersectionBottom = std::max(rBoundingBoxA.getMinY(), rBoundingBoxB.getMinY());
    const float fIntersectionTop    = std::min(rBoundingBoxA.getMaxY(), rBoundingBoxB.getMaxY());

    //------------------------------
    // Extract image data to access raw alpha channels
    auto* pImageA = new Image();
    auto* pImageB = new Image();

    //------------------------------
    // Retrieve texture paths or use a cached bitmask structure to avoid real-time I/O
    pImageA->initWithImageFile(pSpriteA->getName());
    pImageB->initWithImageFile(pSpriteB->getName());

    const unsigned char* pDataA = pImageA->getData();
    const unsigned char* pDataB = pImageB->getData();

    const int imgWidthA  = pImageA->getWidth();
    const int imgHeightA = pImageA->getHeight();
    const int imgWidthB  = pImageB->getWidth();
    const int imgHeightB = pImageB->getHeight();

    constexpr float step = 1.0f;

    //------------------------------
    // Scan the intersection window pixel by pixel
    for ( float fY = fIntersectionBottom; fY < fIntersectionTop; fY += step )
    {
        for ( float fX = fIntersectionLeft; fX < fIntersectionRight; fX += step )
        {
            const Vec2 parentPoint(fX, fY);

            //------------------------------
            // Map world coordinates back to local texture pixels
            const Vec2 vLocalPointA = pSpriteA->convertToNodeSpace(parentPoint);
            const Vec2 vLocalPointB = pSpriteB->convertToNodeSpace(parentPoint);

            //------------------------------
            // Convert to standard image matrix index (invert Y if coordinates are flipped)
            const int iPixelAx = static_cast<int>(vLocalPointA.x);
            const int iPixelAy = imgHeightA - 1 - static_cast<int>(vLocalPointA.y);
            const int iPixelBx = static_cast<int>(vLocalPointB.x);
            const int iPixelBy = imgHeightB - 1 - static_cast<int>(vLocalPointB.y);

            //------------------------------
            // Safely check bounds
            if ( iPixelAx >= 0 && iPixelAx < imgWidthA && iPixelAy >= 0 && iPixelAy < imgHeightA &&
                 iPixelBx >= 0 && iPixelBx < imgWidthB && iPixelBy >= 0 && iPixelBy < imgHeightB )
            {
                //------------------------------
                // Get alpha offset (Assuming RGBA8888 format where alpha is the 4th byte)
                const unsigned char iAlphaA = pDataA[ ( iPixelAy * imgWidthA + iPixelAx ) * 4 + 3 ];
                const unsigned char iAlphaB = pDataB[ ( iPixelBy * imgWidthB + iPixelBx ) * 4 + 3 ];

                //------------------------------
                // If both pixels are opaque, a definitive collision is confirmed
                if ( iAlphaA > 0 && iAlphaB > 0 )
                {
                    AX_SAFE_DELETE( pImageA );
                    AX_SAFE_DELETE( pImageB );

                    return true;
                }
            }
        }
    }

    AX_SAFE_DELETE( pImageA );
    AX_SAFE_DELETE( pImageB );

    return false;
}

void SideShooterScene::onKeyPressed(const EventKeyboard::KeyCode key)
{
    if (_gameOverLbl->isVisible())
        return;

    switch (key)
    {
    case EventKeyboard::KeyCode::KEY_UP_ARROW:
    case EventKeyboard::KeyCode::KEY_LEFT_ARROW:
    case EventKeyboard::KeyCode::KEY_8:
    case EventKeyboard::KeyCode::KEY_4:
        if (_speed == 0)
            _speed = PLAYER_SPEED;

        break;
    
    case EventKeyboard::KeyCode::KEY_DOWN_ARROW:
    case EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
    case EventKeyboard::KeyCode::KEY_2:
    case EventKeyboard::KeyCode::KEY_6:
        if (_speed == 0)
            _speed = -PLAYER_SPEED;

        break;
    
    case EventKeyboard::KeyCode::KEY_SPACE:
        shootBullet();
        break;
    
    default:
        break;
    }
}

void SideShooterScene::onKeyReleased(const EventKeyboard::KeyCode key)
{
    if (_gameOverLbl->isVisible())
        return;

    switch (key)
    {
    case EventKeyboard::KeyCode::KEY_UP_ARROW:
    case EventKeyboard::KeyCode::KEY_LEFT_ARROW:
    case EventKeyboard::KeyCode::KEY_8:
    case EventKeyboard::KeyCode::KEY_4:
        if (_speed == PLAYER_SPEED)
            _speed = 0;

        break;
    
    case EventKeyboard::KeyCode::KEY_DOWN_ARROW:
    case EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
    case EventKeyboard::KeyCode::KEY_2:
    case EventKeyboard::KeyCode::KEY_6:
        if (_speed == -PLAYER_SPEED)
            _speed = 0;

        break;
        
    default:
        break;
    }
}

void SideShooterScene::shootBullet() const
{
    if (_bullet->isVisible())
        return;

    _bullet->setPosition(_player->getBoundingBox().getMaxX(), _player->getPositionY());
    _bullet->setVisible(true);
    playSound("bullet.wav");
}

void SideShooterScene::spawn(const int index) const
{
    int objIndex = 0;
    while (_objects[objIndex]->isVisible())
        ++objIndex;

    if (objIndex >= 8)
        return;

    const auto obj = _objects[objIndex];
    obj->setTexture(OBJECT_TEX[index]);
    obj->setName(OBJECT_TEX[index]);
    obj->setTag(index);
    obj->setPosition(_objectStartX, random(_objectMinY, _objectMaxY));
    obj->setVisible(true);

    if (index == 0)
        obj->setOpacity(128);
    else if (index > 4)
        static_cast<Rotation*>(obj->getUserData())->speed = random(-90.0f, 90.0f);
}

void SideShooterScene::gameOver()
{
    unscheduleUpdate();
    _gameOverLbl->setVisible(true);
}
