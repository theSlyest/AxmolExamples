#include "ShadowShapeScene.h"

#include "Helper.h"

#include "CircleShape.h"
#include "DiamondShape.h"
#include "HeartShape.h"
#include "HexagonShape.h"
#include "OvalShape.h"
#include "PentagonShape.h"
#include "RectangleShape.h"
#include "SquareShape.h"
#include "StarShape.h"
#include "TriangleShape.h"

#include <unordered_set>

USING_NS_AX;

// on "init" you need to initialize your instance
bool ShadowShapeScene::init()
{
    if ( !Scene::init() )
        return false;

    const auto& visibleSize = Director::getInstance()->getVisibleSize();
    const Vec2& origin = Director::getInstance()->getVisibleOrigin();

    // Background
    const auto bgLayer = LayerColor::create(Color4B(208, 228, 250, 255));
    addChild(bgLayer);

    initScore(visibleSize, origin);
    initReset(visibleSize, origin);
    initTitle(visibleSize, origin);
    initMessage(visibleSize, origin);
    initWin(visibleSize, origin);
    initConfetti();

    // Bottom layer
    initBottomLayer(visibleSize, origin);
    initShapes();
    initShadows();

    initKeyboardListener();
    initTouchListener();

    return true;
}

void ShadowShapeScene::initBottomLayer(const Size &visibleSize, const Vec2& origin)
{
    auto bottomLayer = LayerColor::create(Color4B(236, 224, 184, 255), visibleSize.width, 216.0f);
    addChild(bottomLayer);

    auto lineNode = DrawNode::create();
    lineNode->drawLine(Vec2::ZERO, Vec2(visibleSize.width, 0.0f), Color4F(0.58f, 0.48f, 0.3f, 1.0f), 4.0f);
    lineNode->setPosition(origin.x, origin.y +  216.0f);
    addChild(lineNode);

    const auto trayNode = DrawNode::create();
    Helper::drawDashedRoundedRect(trayNode, Vec2::ZERO, Vec2(visibleSize.width - 16.0f, 130.0f),
        16.0f, 8.0f, 6.0f, Color4F(0.58f, 0.48f, 0.3f, 1.0f));
    trayNode->setContentSize(Size(visibleSize.width - 16.0f, 130.0f));
    trayNode->setPosition(origin.x + 8.0f, origin.y + 70.0f);
    addChild(trayNode);

    const auto label = Label::createWithTTF("Drag a shape from the tray and drop it on the matching shadow!", "fonts/arial.ttf", 22.0f);
    label->setTextColor(Color4B(32, 54, 134, 255));
    label->setPosition(origin.x + visibleSize.width / 2.0f, origin.y + 28.0f);
    addChild(label, 2);
}

void ShadowShapeScene::initScore(const Size& visibleSize, const Vec2& origin)
{
    Size size(156.0f, 44.0f);
    const auto drawNode = DrawNode::create();
    Helper::drawRoundedRect(drawNode, Vec2::ZERO, Vec2(size), 10.0f, Color4F::WHITE, Color4F(0.48f, 0.66f, 0.88f, 1.0f));
    drawNode->setPosition(origin.x + 14.0f, origin.y + visibleSize.height - 58.0f);
    drawNode->setContentSize(size);
    addChild(drawNode);

    const auto label = Label::createWithTTF("Score:", "fonts/arial.ttf", 24.0f);
    label->setTextColor(Color4B(32, 54, 134, 255));
    label->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    label->setPosition(30.0f, 22.0f);
    drawNode->addChild(label);

    _scoreLabel = Label::createWithTTF("0", "fonts/arial.ttf", 24.0f);
    _scoreLabel->setTextColor(Color4B(92, 154, 68, 255));
    _scoreLabel->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    _scoreLabel->setPosition(106.0f, 22.0f);
    drawNode->addChild(_scoreLabel);
}

void ShadowShapeScene::initReset(const Size& visibleSize, const Vec2& origin)
{
    Size size(170.0f, 44.0f);
    const auto drawNode = DrawNode::create();
    Helper::drawRoundedRect(drawNode, Vec2::ZERO, Vec2(size), 10.0f, Color4F::WHITE, Color4F(0.48f, 0.66f, 0.88f, 1.0f));
    drawNode->setPosition(origin.x + 776.0f, origin.y + visibleSize.height - 58.0f);
    drawNode->setContentSize(size);
    addChild(drawNode);

    const auto label = Label::createWithTTF("Press R to Reset", "fonts/arial.ttf", 18.0f);
    label->setTextColor(Color4B(32, 54, 134, 255));
    label->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    label->setPosition(16.0f, 22.0f);
    drawNode->addChild(label);
}

void ShadowShapeScene::initTitle(const Size& visibleSize, const Vec2& origin)
{
    const auto label = Label::createWithTTF("Shadow Shape Match", "fonts/arial.ttf", 44.0f);
    label->setTextColor(Color4B(32, 54, 134, 255));
    label->setPosition(origin.x + visibleSize.width / 2.0f, origin.y + visibleSize.height - 36.0f);
    addChild(label);
}

void ShadowShapeScene::initMessage(const Size& visibleSize, const Vec2& origin)
{
    _msgLabel = Label::createWithTTF("", "fonts/arial.ttf", 34.0f);
    _msgLabel->setTextColor(Color4B(91, 152, 66, 255));
    _msgLabel->setPosition(origin.x + visibleSize.width / 2.0f, origin.y + visibleSize.height - 88.0f);
    addChild(_msgLabel);
}


void ShadowShapeScene::initWin(const Size& visibleSize, const Vec2& origin)
{
    _winLabel = Label::createWithTTF("Great job!", "fonts/arial.ttf", 48.0f);
    _winLabel->setTextColor(Color4B(91, 152, 66, 255));
    _winLabel->setPosition(origin.x + visibleSize.width / 2.0f, origin.y + visibleSize.height / 2.0f);
    _winLabel->setVisible(false);
    addChild(_winLabel, 2);
}

void ShadowShapeScene::setScore(const int score)
{
    _score = score;
    _scoreLabel->setString(std::to_string(score));
}

void ShadowShapeScene::setMessage(const std::string& msg) const
{
    _msgLabel->setString(msg);
}

void ShadowShapeScene::addShape(Shape* shape)
{
    addChild(shape);
    _shapes.push_back(shape);
}

void ShadowShapeScene::resetShapes() 
{
    _matchCount = 0;
    for (int i = 0; i < _shapes.size(); ++i) {
        _shapes[i]->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
        const float x = (i == 0 ? 24.0f : _shapes[i - 1]->getBoundingBox().getMaxX() + 18.0f);
        _shapes[i]->setPosition(x, 135.0f);
        _shapes[i]->setTouchable(true);
        _shapes[i]->setScale(1.0f);
        _shapes[i]->setLocalZOrder(0);
    }
}

void ShadowShapeScene::initShapes()
{
    _movedShape = nullptr;
    addShape(CircleShape::create(false));
    addShape(SquareShape::create(false));
    addShape(TriangleShape::create(false));
    addShape(RectangleShape::create(false));
    addShape(StarShape::create(false));
    addShape(HeartShape::create(false));
    addShape(DiamondShape::create(false));
    addShape(OvalShape::create(false));
    addShape(PentagonShape::create(false));
    addShape(HexagonShape::create(false));
    resetShapes();
}

void ShadowShapeScene::initShadows()
{
    _shadows.assign(6, nullptr);
    _shadowGenerators = {
        { ShapeType::Circle, []() { return CircleShape::create(true); } },
        { ShapeType::Diamond, []() { return DiamondShape::create(true); } },
        { ShapeType::Heart, []() { return HeartShape::create(true); } },
        { ShapeType::Hexagon, []() { return HexagonShape::create(true); } },
        { ShapeType::Oval, []() { return OvalShape::create(true); } },
        { ShapeType::Pentagon, []() { return PentagonShape::create(true); } },
        { ShapeType::Rectangle, []() { return RectangleShape::create(true); } },
        { ShapeType::Square, []() { return SquareShape::create(true); } },
        { ShapeType::Star, []() { return StarShape::create(true); } },
        { ShapeType::Triangle, []() { return TriangleShape::create(true); } },
    };
    resetShadows();
}

void ShadowShapeScene::resetShadows()
{
    for (const auto shape : _shadows) {
        removeChild(shape);
    }

    std::unordered_set<ShapeType> shapeTypes;
    while (shapeTypes.size() < 6) {
        shapeTypes.insert(static_cast<ShapeType>(random(0, static_cast<int>(ShapeType::count) - 1)));
    }

    for (int i = 0; i < 6; ++i) {
        const float x = 200.0f + i * 114.0f;
        const float y = i % 2 == 0 ? 452.0f : 308.0f;
        ShapeType type = *(shapeTypes.begin());
        const auto shadow = _shadowGenerators[type]();
        shadow->setScale(ShadowScale);
        shadow->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        shadow->setPosition(x, y);
        addChild(shadow);
        _shadows[i] = shadow;
        shapeTypes.erase(type);
    }
}

void ShadowShapeScene::initKeyboardListener()
{
    _keyListener = EventListenerKeyboard::create();
    _keyListener->onKeyPressed = [this](EventKeyboard::KeyCode key, Event* event) { onKeyPressed(key); };
    _eventDispatcher->addEventListenerWithSceneGraphPriority(_keyListener, this);
}

void ShadowShapeScene::initTouchListener()
{
    _touchListener = EventListenerTouchOneByOne::create();
    _touchListener->onTouchBegan = [this](const Touch* touch, Event*) -> bool { return onTouchBegan(touch); };
    _touchListener->onTouchMoved = [this](const Touch* touch, Event*) { onTouchMoved(touch); };
    _touchListener->onTouchEnded = [this](Touch*, Event*) { onTouchEnded(); };
    _eventDispatcher->addEventListenerWithSceneGraphPriority(_touchListener, this);
}

void ShadowShapeScene::onKeyPressed(const EventKeyboard::KeyCode key)
{
    if (key == EventKeyboard::KeyCode::KEY_R) {
        hardReset();
    }
}

bool ShadowShapeScene::onTouchBegan(const Touch* touch)
{
    const Vec2& location = touch->getLocation();
    for (const auto shape : _shapes) {
        if (shape->isTouchable() && shape->getBoundingBox().containsPoint(location)) {
            _movedShape = shape;
            _movedShape->setLocalZOrder(1);
            _movedOriginalPos = shape->getPosition();
            _touchOffset = _movedOriginalPos - location;
            return true;
        }
    }
    return false;
}

void ShadowShapeScene::onTouchMoved(const Touch* touch) const
{
    if (_movedShape != nullptr) {
        _movedShape->setPosition(touch->getLocation() + _touchOffset);
    }
}

void ShadowShapeScene::onTouchEnded()
{
    bool matched = false;
    const Vec2 movedPos = _movedShape->getCenter();
    const ShapeType movedType = _movedShape->getType();
    for (const auto shadow : _shadows) {
        if (const Vec2 shadowPos = shadow->getCenter(); movedPos.distance(shadowPos) < 48.0f) {
            if (movedType == shadow->getType()) {
                _movedShape->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
                _movedShape->setPosition(shadowPos);
                _movedShape->setTouchable(false);
                _movedShape->setScale(ShadowScale);
                matched = true;
                displayMessage("Good job!");
                _previousFail = false;
                setScore(_score + 1);
                _matchCount++;
                if (_matchCount == 6) {
                    celebrate();
                }
            }
            else {
                displayMessage("Try again!");
                if (_previousFail) {
                    for (const auto shadow2 : _shadows) {
                        if (shadow2->getType() == movedType) {
                            shadow2->highlight();
                            _previousFail = false;
                            break;
                        }
                    }
                }
                else {
                    _previousFail = true;
                }
            }

            break;
        }
    }

    if (!matched) {
        _movedShape->setPosition(_movedOriginalPos);
    }

    _movedShape->setLocalZOrder(0);
    _movedShape = nullptr;
}

void ShadowShapeScene::softReset() 
{
    _winLabel->setVisible(false);
    _touchListener->setEnabled(true);
    resetShapes();
    resetShadows();
}

void ShadowShapeScene::hardReset() 
{
    unschedule("eraseMessage");
    unschedule("softReset");
    setMessage("");
    setScore(0);
    _confetti->stop();
    softReset();
}

void ShadowShapeScene::displayMessage(const std::string& msg)
{
    unschedule("eraseMessage");
    setMessage(msg);
    scheduleOnce([this](float) { setMessage(""); }, 2.0f, "eraseMessage");
}

void ShadowShapeScene::celebrate() 
{
    _winLabel->setVisible(true);
    _touchListener->setEnabled(false);
    _confetti->celebrate();
    scheduleOnce([this](float) { softReset(); }, 2.0f, "softReset");
}

void ShadowShapeScene::initConfetti()
{
    _confetti = ConfettiNode::create();
    addChild(_confetti, 2);
}