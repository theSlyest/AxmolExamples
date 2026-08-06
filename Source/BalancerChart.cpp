#include "BalancerChart.h"

USING_NS_AX;

BalancerChart *BalancerChart::create(const float tolerance)
{
    if (auto* pRet = new (std::nothrow) BalancerChart(); pRet && pRet->init(tolerance))
    {
        pRet->autorelease();
        return pRet;
    }
    else
    {
        delete pRet;
        pRet = nullptr;
        return nullptr;
    }
}

bool BalancerChart::init(const float tolerance)
{
    if(!DrawNode::init())
        return false;

    const auto& size = _director->getVisibleSize();
    setContentSize(Size(size.width - 64.0f, 200.0f));
    // setLineWidth(LINE);

    _toleranceWidth = tolerance * _contentSize.height;

    // Background
    drawSolidRect(Vec2::ZERO, Vec2(_contentSize), BG_COLOR);
    for (int i = 1; i < 4; ++i) {
        const float y = i * _contentSize.height / 4.0f;
        drawLine(Vec2(0.0f, y), Vec2(_contentSize.width, y), LINE_COLOR, LINE);
    }

    // LEGEND
    const auto legend = DrawNode::create(); // CHART
    legend->drawLine(Vec2::ZERO, Vec2(50.0f, 0.0f), DEMAND_COLOR, CHART);
    legend->drawLine(Vec2(200.0f, 0.0f), Vec2(250.0f, 0.0f), SUPPLY_COLOR, CHART);
    legend->drawSolidRect(Vec2(400.0f, -_toleranceWidth), Vec2(450.0f, _toleranceWidth), BAND_COLOR);
    legend->setPosition(16.0f, _contentSize.height + 32.0f);
    const auto dLbl = Label::createWithTTF("demand", "fonts/arial.ttf", 18.0f);
    dLbl->setTextColor(Color4B::GRAY);
    dLbl->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT),
    dLbl->setPosition(60.0f, 0.0f);
    legend->addChild(dLbl);
    const auto sLbl = Label::createWithTTF("supply", "fonts/arial.ttf", 18.0f);
    sLbl->setTextColor(Color4B::GRAY);
    sLbl->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT),
    sLbl->setPosition(260.0f, 0.0f);
    legend->addChild(sLbl);
    const auto bLbl = Label::createWithTTF("tolerance band", "fonts/arial.ttf", 18.0f);
    bLbl->setTextColor(Color4B::GRAY);
    bLbl->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT),
    bLbl->setPosition(460.0f, 0.0f);
    legend->addChild(bLbl);
    addChild(legend);

    const auto stencil = DrawNode::create();
    stencil->setContentSize(_contentSize);
    stencil->drawSolidRect(Vec2::ZERO, _contentSize, Color4F::WHITE);
    const auto clipper = ClippingNode::create(stencil);
    clipper->setInverted(false);
    addChild(clipper);
    
    _charts = Node::create();
    _charts->setContentSize(_contentSize);
    clipper->addChild(_charts);

    _bandChart = DrawNode::create();
    _bandChart->setContentSize(_contentSize);
    _charts->addChild(_bandChart);

    _demandChart = DrawNode::create();
    _demandChart->setContentSize(_contentSize);
    _charts->addChild(_demandChart);

    _supplyChart = DrawNode::create();
    _supplyChart->setContentSize(_contentSize);
    _charts->addChild(_supplyChart);

    _graceNode = DrawNode::create();
    _graceNode->drawCircle(Vec2(GRACE_RADIUS, GRACE_RADIUS), GRACE_RADIUS, 0.0f, 32, false, Color4F::ORANGE, BORDER);
    _graceNode->drawSolidRect(Vec2(-8.0f, 8.0f), Vec2(-68.0f, 38.0f), Color4F::BLACK);
    _graceNode->drawRect(Vec2(-8.0f, 8.0f), Vec2(-68.0f, 38.0f), Color4F::ORANGE, BORDER);

    _graceLbl = Label::createWithTTF("0.0s", "fonts/arial.ttf", 18.0f);
    _graceLbl->setTextColor(Color4B::ORANGE);
    _graceLbl->setPosition(-38.0f, 23.0f);
    _graceNode->addChild(_graceLbl);

    _graceNode->setContentSize(Size(2 * GRACE_RADIUS, 2 * GRACE_RADIUS));
    _graceNode->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    _graceNode->setVisible(false);
    _charts->addChild(_graceNode);

    return true;
}

void BalancerChart::update(const float dt)
{
    _elapsed += dt;
    _periodTimer += dt;

    if (_periodTimer >= PERIOD_DELAY) {
        _periodTimer -= PERIOD_DELAY;
        _period = std::max(PERIOD_MIN, _period - PERIOD_STEP);
    }

    constexpr float twoPi = M_PI * 2.0f;
    _phase += dt * twoPi / _period;
    if (_phase >= twoPi)
        _phase = fmod(_phase, twoPi);

    const float sinVal = sin(_phase);
    if (const int cosSign = (cos(_phase) >= 0.0f) ? 1 : -1; cosSign != _sinSign) {
        const bool isPeak = (sinVal > 0.0f);
        onExtremumReached(isPeak);
        _sinSign = cosSign;
    }

    const float dy = clampf(_midLine + sinVal * _amplitude / 2.0f, 0.0f, 1.0f) * _contentSize.height;

    const float shiftX = dt * SPEED_X;
    const float x = _xt + shiftX;
    const float sy = _supply * _contentSize.height;
    const Vec2 dStart(_xt, _demandY);
    const Vec2 dEnd(x, dy);
    _bandChart->drawSolidRect(Vec2(_xt, _demandY - _toleranceWidth), Vec2(x, dy + _toleranceWidth), BAND_COLOR);
    _demandChart->drawLine(dStart, dEnd, DEMAND_COLOR, CHART);
    const Vec2 supplyPos(x, sy);
    _supplyChart->drawLine(Vec2(_xt, _supplyY), supplyPos, SUPPLY_COLOR, CHART);
    _graceNode->setPosition(supplyPos);
    _xt = x;
    _demandY = dy;
    _supplyY = sy;
    if (_xt > _contentSize.width * 0.96f) 
        _charts->setPositionX(_charts->getPositionX() - shiftX);
}

void BalancerChart::reset(const float initialValue)
{
    _bandChart->clear();
    _demandChart->clear();
    _supplyChart->clear();
    _charts->setPositionX(0.0f);
    _demandY = _contentSize.height * initialValue;
    _supplyY = _demandY;
    _supply = initialValue;

    _xt = 0.0f;
    _amplitude = 0.0f;
    _midLine = initialValue;
    _period = PERIOD_START;
    _sinSign = -1;
    _phase = 1.5f * M_PI;
    onExtremumReached(false);
}

void BalancerChart::setGraceTime(const float time) const
{
    _graceLbl->setString(StringUtils::format("%.1fs", time));
}

void BalancerChart::onExtremumReached(const bool isPeak)
{
    const float halfAmp = _amplitude / 2.0f;
    if (isPeak) {
        const float extremumY = _midLine + halfAmp;
        _amplitude = random(std::min(MIN_AMP, extremumY), std::min(MAX_AMP, extremumY));
        _midLine = extremumY - _amplitude / 2.0f;
    }
    else {
        const float extremumY = _midLine - halfAmp;
        const float maxAmp = 1.0f - extremumY;
        _amplitude = random(std::min(MIN_AMP, maxAmp), std::min(MAX_AMP, maxAmp));
        _midLine = extremumY + _amplitude / 2.0f;
    }
}
