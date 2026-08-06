#include "BalancerScene.h"

USING_NS_AX;

Scene* BalancerScene::createScene()
{
    return BalancerScene::create();
}

// on "init" you need to initialize your instance
bool BalancerScene::init()
{
    if ( !Scene::init() )
        return false;

    const auto& size = Director::getInstance()->getVisibleSize();
    const Vec2& origin = Director::getInstance()->getVisibleOrigin();

    // UPTIME
    _upTimeUi = BalancerValue::create(Color4F(0.2f, 0.6f, 1.0f, 1.0f), "UPTIME");
    _upTimeUi->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
    _upTimeUi->setPosition(origin.x + UI_OFFSET, origin.y + size.height - UI_OFFSET);
    addChild(_upTimeUi);

    // STRIKES
    _strikesUi = BalancerValue::create(Color4F(0.4f, 1.0f, 0.4f, 1.0f), "STRIKES");
    _strikesUi->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    _strikesUi->setPosition(origin.x + size.width / 2.0f, origin.y + size.height - UI_OFFSET);
    addChild(_strikesUi);

    // BEST
    _bestUi = BalancerValue::create(Color4F(1.0f, 1.0f, 0.2f, 1.0f), "BEST");
    _bestUi->setAnchorPoint(Vec2::ANCHOR_TOP_RIGHT);
    _bestUi->setPosition(origin.x + size.width - UI_OFFSET, origin.y + size.height - UI_OFFSET);
    addChild(_bestUi);

    setBest(UserDefault::getInstance()->getIntegerForKey(BEST_KEY, 0));

    // GENERATORS
    initGenerators(size, origin);

    // CHART
    _chart = BalancerChart::create(TOLERANCE);
    _chart->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    _chart->setPosition(origin.x + size.width / 2.0f, _upTimeUi->getBoundingBox().getMinY() - 3.0f * UI_OFFSET);
    addChild(_chart);

    // GAME OVER LAYER
    initEndLayer(size, origin);

    reset();

    return true;
}

void BalancerScene::update(float dt)
{
    float supply = 0.0f;
    for (auto gen : _gens) {
        gen->update(dt);
        supply += gen->getValue();
    }
    supply /= GENS;
    _chart->setSupply(supply);
    _chart->update(dt);
    const float demand = _chart->getDemand();
    if (abs(supply - demand) > TOLERANCE) {
        _downTime += dt;
        _chart->setGraceTime(_downTime);
        _chart->setGraceVisible(true);
        if (_downTime >= GRACE_PERIOD) {
            setStrikes(_strikes - 1);
            _downTime -= GRACE_PERIOD;
            if (_strikes == 0) {
                gameOver();
                return;
            }
        }
    }
    else {
        _chart->setGraceVisible(false);
        _downTime = 0.0f;
        setUpTime(_upTime + dt);
    }
}

void BalancerScene::setUpTime(float upTime)
{
    _upTime = upTime;
    _upTimeUi->setValue(StringUtils::format("%03ds", static_cast<int>(upTime)));
}

void BalancerScene::setStrikes(int strikes)
{
    _strikes = strikes;
    _strikesUi->setValue(std::to_string(strikes));
}

void BalancerScene::setBest(int best)
{
    _best = best;
    _bestUi->setValue(StringUtils::format("%03ds", best));
}

void BalancerScene::initGenerators(const Size& size, const Vec2& origin)
{
    const float offset = size.width / (GENS * 2.0f);
    for (int i = 0; i < GENS; ++i) {
        const auto gen = BalancerGenerator::create(GEN_COLORS[i], GEN_NAMES[i], GEN_RATES[i]);
        gen->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
        gen->setPosition(origin.x + offset + i * size.width / GENS, UI_OFFSET);
        gen->resetValue(0.5f);
        addChild(gen);
        _gens.push_back(gen);
    }
}

void BalancerScene::initEndLayer(const ax::Size &size, const ax::Vec2 &origin)
{
    const Size endSize(400.0f, 300.0f);
    _endLayer = DrawNode::create();
    _endLayer->setContentSize(endSize);
    _endLayer->drawSolidRect(Vec2::ZERO, endSize, Color4F(0.1f, 0.15f, 0.2f, 1.0f));
    _endLayer->drawRect(Vec2::ZERO, endSize, Color4F(1.0f, 0.6f, 0.0f, 1.0f), 4.0f);
    _endLayer->setVisible(false);
    _endLayer->setPosition(origin + Vec2(size - endSize) / 2.0f);
    addChild(_endLayer, 1);

    const auto out = Label::createWithTTF("Out of strikes", "fonts/arial.ttf", 24.0f);
    out->setTextColor(Color4B(255, 64, 0, 255));
    out->setPosition(endSize.width / 2.0f, 240.0f);
    _endLayer->addChild(out);

    _upTimeLbl = Label::createWithTTF("Uptime banked: 000s", "fonts/arial.ttf", 18.0f);
    _upTimeLbl->setPosition(endSize.width / 2.0f, 180.0f);
    _endLayer->addChild(_upTimeLbl);

    _bestLbl = Label::createWithTTF("Best: 000s", "fonts/arial.ttf", 24.0f);
    _bestLbl->setTextColor(Color4B(224, 224, 0, 255));
    _bestLbl->setPosition(endSize.width / 2.0f, 120.0f);
    _endLayer->addChild(_bestLbl);

    const auto again = Label::createWithTTF("Play Again", "fonts/arial.ttf", 18.0f);
    const auto item = MenuItemLabel::create(again, [this](Object*) { reset(); });
    const Size itemSize = item->getContentSize() + Size(48.0f, 24.0f);

    const auto bg = DrawNode::create();
    bg->setContentSize(itemSize);
    bg->drawSolidRect(Vec2::ZERO, itemSize, Color4F(0.25f, 0.3f, 0.35f, 1.0f));
    bg->drawRect(Vec2::ZERO, itemSize, Color4F(0.2f, 0.6f, 1.0f, 1.0f), 3.0f);
    bg->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    bg->setPosition(item->getContentSize() / 2.0f);
    item->addChild(bg, -1);

    auto menu = Menu::create(item, nullptr);
    menu->setPosition(endSize.width / 2.0f, 60.0f);
    _endLayer->addChild(menu);
}

void BalancerScene::gameOver()
{
    unscheduleUpdate();
    int intTime = static_cast<int>(_upTime);
    _upTimeLbl->setString(StringUtils::format("Uptime banked: %03ds", intTime));

    if (intTime > _best) {
        auto userDef = UserDefault::getInstance();
        userDef->setIntegerForKey(BEST_KEY, intTime);
        userDef->flush();
        setBest(intTime);
    }

    _bestLbl->setString(StringUtils::format("Best: %03ds", _best));

    _endLayer->setVisible(true);
}

void BalancerScene::reset()
{
    _demandGoal = GEN_START;
    _chart->reset(GEN_START);
    setStrikes(STRIKES);
    for (auto gen : _gens)
        gen->resetValue(GEN_START);

    _demandIncrease = random(0, 1);
    _demandFactor = 0.3f;
    _demandGoal = GEN_START;
    _endLayer->setVisible(false);
    scheduleUpdate();
}
