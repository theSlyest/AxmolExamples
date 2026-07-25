#include "CookingScene.h"

USING_NS_AX;

const std::map<Ingredient, std::vector<CookingState>> CookingScene::CookingStates = {
    { Ingredient::Bread,   { { .duration = 4.0f, .color = Color3B(255, 255, 255) }, { .duration = 2.0f, .color = Color3B(201, 177, 120) } } },
    { Ingredient::Chicken, { { .duration = 6.0f, .color = Color3B(255, 192, 203) }, { .duration = 2.0f, .color = Color3B(201, 177, 120) } } },
    { Ingredient::Bacon,   { { .duration = 5.0f, .color = Color3B(255, 192, 203) }, { .duration = 1.5f, .color = Color3B(255, 0, 0) } } },
    { Ingredient::Onions,  { { .duration = 5.0f, .color = Color3B(255, 255, 255) }, { .duration = 2.0f, .color = Color3B(201, 177, 120) } } },
};

// on "init" you need to initialize your instance
bool CookingScene::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Scene::init() )
        return false;

    const auto& visibleSize = Director::getInstance()->getVisibleSize();
    const Vec2& origin = Director::getInstance()->getVisibleOrigin();

    const auto bg = LayerColor::create(Color4B(182, 127, 67, 255));
    addChild(bg);

    initSuccessful(visibleSize, origin);
    initAttempts(visibleSize, origin);
    initTimer(visibleSize, origin);
    initFryingPan(visibleSize, origin);
    initIngredients(visibleSize, origin);
    initResult(visibleSize, origin);

    initTouchListener();

    return true;
}

void CookingScene::update(const float dt)
{
    updateTimer(dt);
}

void CookingScene::initSuccessful(const Size &visibleSize, const Vec2 &origin)
{
    _successful = 0;

    Size size(256.0f, 80.0f);
    auto bg = RoundedRect::create(size, 12.0f, Color4F(0.0f, 0.0f, 0.0f, 0.8f));
    bg->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
    bg->setPosition(origin.x + 16.0f, origin.y + visibleSize.height - 16.0f);
    addChild(bg);

    auto caption = Label::createWithTTF("Successful Sandwiches:", "fonts/arial.ttf", 20.0f);
    caption->enableBold();
    caption->setPosition(size.width / 2.0f, size.height - 20.0f);
    bg->addChild(caption);

    _successfulLabel = Label::createWithTTF("0", "fonts/arial.ttf", 44.0f);
    _successfulLabel->setTextColor(Color4B(32, 248, 2, 255));
    _successfulLabel->setPosition(size.width / 2.0f, 26.0f);
    bg->addChild(_successfulLabel);
}

void CookingScene::initAttempts(const Size &visibleSize, const Vec2 &origin)
{
    _attempts = 0;

    constexpr Size size(256.0f, 80.0f);
    const auto bg = RoundedRect::create(size, 12.0f, Color4F(0.0f, 0.0f, 0.0f, 0.8f));
    bg->setAnchorPoint(Vec2::ANCHOR_TOP_RIGHT);
    bg->setPosition(origin.x + visibleSize.width - 16.0f, origin.y + visibleSize.height - 16.0f);
    addChild(bg);

    const auto caption = Label::createWithTTF("Total Attempts:", "fonts/arial.ttf", 20.0f);
    caption->enableBold();
    caption->setPosition(size.width / 2.0f, size.height - 20.0f);
    bg->addChild(caption);

    _attemptsLabel = Label::createWithTTF("0", "fonts/arial.ttf", 44.0f);
    _attemptsLabel->setTextColor(Color4B(14, 242, 242, 255));
    _attemptsLabel->setPosition(size.width / 2.0f, 26.0f);
    bg->addChild(_attemptsLabel);
}

void CookingScene::initTimer(const Size &visibleSize, const Vec2 &origin)
{
    _timer = 0.0f;

    constexpr Size size(228.0f, 48.0f);
    _timerPanel = RoundedRect::create(size, 12.0f, Color4F(0.0f, 0.0f, 0.0f, 0.8f));
    _timerPanel->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    _timerPanel->setPosition(origin.x + visibleSize.width / 2.0f, origin.y + visibleSize.height - 16.0f);
    _timerPanel->setVisible(false);
    addChild(_timerPanel);

    const auto caption = Label::createWithTTF("Timer:", "fonts/arial.ttf", 32.0f);
    caption->enableBold();
    caption->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    caption->setPosition(22.0f, size.height / 2.0f);
    _timerPanel->addChild(caption);

    _timerLabel = Label::createWithTTF("0.00s", "fonts/arial.ttf", 32.0f);
    _timerLabel->setTextColor(Color4B(244, 242, 4, 255));
    _timerLabel->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
    _timerLabel->setPosition(size.width - 22.0f, size.height / 2.0f);
    _timerPanel->addChild(_timerLabel);
}

void CookingScene::initFryingPan(const Size &visibleSize, const Vec2 &origin)
{
    _pan = FryingPan::create();
    _pan->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    _pan->setPosition(origin.x + visibleSize.width / 2.0f, origin.y + visibleSize.height / 2.0f);
    addChild(_pan);
}

void CookingScene::initIngredients(const Size &visibleSize, const Vec2 &origin)
{
    constexpr int count = static_cast<int>(Ingredient::count);

    const float btnY = origin.y + 92.0f;
    constexpr float btnInterval = IngredientButton::Side + 32.0f;
    const float btnOffsetX = origin.x + 160.0f;
    _btnSpriteY = btnY + 10.0f;

    const float slotX = origin.x + visibleSize.width - 128.0f;
    constexpr float slotInterval = 2.0f * Slot::Radius + 48.0f;
    const float slotOffSetY = origin.y + visibleSize.height - 192.0f;

    for (int i = 0; i < count; ++i) {
        const float btnX = btnOffsetX + i * btnInterval;

        auto ingredient = static_cast<Ingredient>(i);
        auto button = IngredientButton::create(static_cast<Ingredient>(i));
        button->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        button->setPosition(btnX, btnY);
        addChild(button);
        _ingredientButtons.push_back(button);

        auto sprite = button->createSprite(152.0f);
        sprite->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        sprite->setPosition(btnX, _btnSpriteY);
        addChild(sprite, 1);
        _ingredientSprites.push_back(sprite);

        const float slotY = slotOffSetY - slotInterval * i;
        auto slot = Slot::create(ingredient);
        slot->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        slot->setPosition(slotX, slotY);
        addChild(slot);
        _slots.push_back(slot);

        const auto slotLabel = Label::createWithTTF(std::to_string(i + 1) + ". " + IngredientNames.at(ingredient), "fonts/arial.ttf", 24.0f);
        slotLabel->enableBold();
        slotLabel->setPosition(slotX, slotY + 56.0f);
        addChild(slotLabel);
    }
}

void CookingScene::initResult(const Size &visibleSize, const Vec2 &origin)
{
    Size size(500.0f, 500.0f);
    _resultPanel = RoundedRect::create(size, 16.0f, Color4F(0.0f, 0.0f, 0.0f, 0.8f));
    _resultPanel->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    _resultPanel->setPosition(origin.x + visibleSize.width / 2.0f, origin.y + visibleSize.height / 2.0f);
    _resultPanel->setVisible(false);
    addChild(_resultPanel, 2);

    _resultLabel = Label::createWithTTF("0/4", "fonts/arial.ttf", 80.0f);
    _resultLabel->setPosition(size.width / 2.0f, size.height / 2.0f);
    _resultPanel->addChild(_resultLabel);
}

void CookingScene::initTouchListener()
{
    _draggedSprite = nullptr;

    _touchListener = EventListenerTouchOneByOne::create();
    _touchListener->onTouchBegan = [this](const Touch* touch, Event*) ->bool { return onTouchBegan(touch); };
    _touchListener->onTouchMoved = [this](Touch* touch, Event*) { onTouchMoved(touch); };
    _touchListener->onTouchEnded = [this](Touch*, Event*) { onTouchEnded(); };
    _eventDispatcher->addEventListenerWithSceneGraphPriority(_touchListener, this);
}

bool CookingScene::onTouchBegan(const Touch *touch)
{
    const Vec2& touchLoc = touch->getLocation();
    for (const auto sprite : _ingredientSprites) {
        if (sprite->getBoundingBox().containsPoint(touchLoc)) {
            if (sprite == _cookedSprite) { // Pan
                pauseCooking();
                _draggedSprite = sprite;
                _touchOffset = sprite->getPosition() - touchLoc;
                return true;
            }
            else if (sprite->getPositionY() == _btnSpriteY) { // Buttons
                _draggedSprite = sprite;
                _touchOffset = sprite->getPosition() - touchLoc;
                return true;
            }
        }
    }

    return false;
}

void CookingScene::onTouchMoved(Touch *touch)
{
    if (_draggedSprite != nullptr) {
        _draggedSprite->setPosition(touch->getLocation() + _touchOffset);
    }
}

void CookingScene::onTouchEnded()
{
    if (_draggedSprite == nullptr)
        return;

    if (_draggedSprite == _cookedSprite) {
        auto slot = _slots[_draggedSprite->getTag()];
        if (slot->getPosition().distance(_draggedSprite->getPosition()) <= 64.0f) {                
            _draggedSprite->setPosition(slot->getPosition());
            _draggedSprite->setScale(72.0f / _draggedSprite->getContentSize().width);
            slot->fill();
            stopCooking();
        }
        else {
            _draggedSprite->setPosition(_pan->getPosition());
            resumeCooking();
        }
    }
    else if (_pan->getBoundingBox().containsPoint(_draggedSprite->getPosition())) {
        _draggedSprite->setPosition(_pan->getPosition());
        _draggedSprite->setScale(304.0f / _draggedSprite->getContentSize().width);
        startCooking();
    }
    else {
        const int index = _draggedSprite->getTag();
        _draggedSprite->setPosition(_ingredientButtons[index]->getPosition());
    }
    _draggedSprite = nullptr;    
}

void CookingScene::setSuccessful(int successful)
{
    _successful = successful;
    _successfulLabel->setString(std::to_string(successful));
}

void CookingScene::setAttempts(int attempts)
{
    _attempts = attempts;
    _attemptsLabel->setString(std::to_string(attempts));
}

void CookingScene::updateTimer(float dt)
{
    _timer += dt;
    _timerLabel->setString(StringUtils::format("%.2fs", _timer));
}

void CookingScene::startCooking()
{
    _cookingLevel = 0;
    _timer = 0.0f;
    _isCooking = true;
    _cookedSprite = _draggedSprite;
    _timerPanel->setVisible(true);
    const auto ingredient = static_cast<Ingredient>(_cookedSprite->getTag());
    _cookedSprite->setColor(CookingStates.at(ingredient)[0].color);
    const auto cookRaw = TintTo::create(CookingStates.at(ingredient)[0].duration, CookingStates.at(ingredient)[1].color);
    const auto nextLevel = CallFunc::create([this]() { ++_cookingLevel; });
    const auto cookPerfect = TintTo::create(CookingStates.at(ingredient)[1].duration, Color3B(16, 16, 16));
    const auto cookSequence = Sequence::create(cookRaw, nextLevel, cookPerfect, nextLevel->clone(), nullptr);
    _cookedSprite->runAction(cookSequence);
    scheduleUpdate();
}

void CookingScene::pauseCooking()
{
    _cookedSprite->pause();
    unscheduleUpdate();
}

void CookingScene::resumeCooking()
{
    _cookedSprite->resume();
    scheduleUpdate();
}

void CookingScene::stopCooking()
{
    _cookedSprite->stopAllActions();
    unscheduleUpdate();
    _timerPanel->setVisible(false);
    _cookedSprite = nullptr;
    if (_cookingLevel == 1) ++_points;
    if (checkAllCooked())
        displayResult();
}

bool CookingScene::checkAllCooked() const
{
    for (auto& slot : _slots) {
        if (!slot->isFull())
            return false;
    }

    return true;
}

void CookingScene::displayResult()
{
    setAttempts(_attempts + 1);
    if (_points == 4)
        setSuccessful(_successful + 1);

    _resultLabel->setString(std::to_string(_points) + "/4");
    _resultPanel->setVisible(true);
    _touchListener->setEnabled(false);
    scheduleOnce([this](float dt) { reset(); }, 3.0f, "Reset");
}

void CookingScene::reset()
{
    _points = 0;
    for (int i = 0; i < static_cast<int>(Ingredient::count); ++i) {
        const auto sprite = _ingredientSprites[i];
        sprite->setColor(Color3B(255, 255, 255));
        sprite->setPosition(_ingredientButtons[i]->getPositionX(), _btnSpriteY);
        sprite->setScale(152.0f / sprite->getContentSize().width);
        _slots[i]->empty();
    }
    _resultPanel->setVisible(false);
    _touchListener->setEnabled(true);
}
