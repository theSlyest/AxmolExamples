#include "BalancerChart.h"

#include "Director.h"
#include "fmt/format.h"
#include "2d/ClippingNode.h"

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

    _toleranceWidth = tolerance * _contentSize.height;

    // Enough samples to fill the chart up to the head position.
    const float dx = SPEED_X * SAMPLE_INTERVAL;
    _capacity = static_cast<size_t>(std::ceil(_contentSize.width * HEAD_X / dx)) + 2;

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
    addLegendLabel(legend, "demand", 60.0f);
    addLegendLabel(legend, "supply", 260.0f);
    addLegendLabel(legend, "tolerance band", 460.0f);
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

void BalancerChart::addLegendLabel(Node* legend, const std::string& text, const float x)
{
    const auto label = Label::createWithTTF(text, "fonts/arial.ttf", 18.0f);
    label->setTextColor(Color4B::GRAY);
    label->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    label->setPosition(x, 0.0f);
    legend->addChild(label);
}

void BalancerChart::update(const float dt, const float demand, const float supply)
{
    _demandY = demand * _contentSize.height;
    _supplyY = supply * _contentSize.height;

    _sampleTimer += dt;
    if (_sampleTimer >= SAMPLE_INTERVAL) {
        while (_sampleTimer >= SAMPLE_INTERVAL) {
            _sampleTimer -= SAMPLE_INTERVAL;
            addSample();
        }
        redraw();
    }

    _graceNode->setPosition(_headX, _supplyY);
}

void BalancerChart::addSample()
{
    _history.push_back({ _demandY, _supplyY });
    if (_history.size() > _capacity)
        _history.pop_front();

    ++_sampleIndex;
    _headX = std::min(static_cast<float>(_sampleIndex) * SPEED_X * SAMPLE_INTERVAL, _contentSize.width * HEAD_X);
}

void BalancerChart::redraw() const
{
    _bandChart->clear();
    _demandChart->clear();
    _supplyChart->clear();

    // The newest sample sits at _headX; older ones are spaced dx apart to its left.
    const float dx = SPEED_X * SAMPLE_INTERVAL;
    const size_t last = _history.size() - 1;
    for (size_t i = 1; i <= last; ++i) {
        const Sample& from = _history[i - 1];
        const Sample& to = _history[i];
        const float x0 = _headX - static_cast<float>(last - (i - 1)) * dx;
        const float x1 = _headX - static_cast<float>(last - i) * dx;

        _bandChart->drawSolidRect(Vec2(x0, from.demand - _toleranceWidth), Vec2(x1, to.demand + _toleranceWidth), BAND_COLOR);
        _demandChart->drawLine(Vec2(x0, from.demand), Vec2(x1, to.demand), DEMAND_COLOR, CHART);
        _supplyChart->drawLine(Vec2(x0, from.supply), Vec2(x1, to.supply), SUPPLY_COLOR, CHART);
    }
}

void BalancerChart::reset(const float initialValue)
{
    _demandY = _contentSize.height * initialValue;
    _supplyY = _demandY;

    _history.clear();
    _history.push_back({ _demandY, _supplyY });
    _sampleTimer = 0.0f;
    _sampleIndex = 0;
    _headX = 0.0f;
    redraw();

}

void BalancerChart::setGraceTime(const float time) const
{
    _graceLbl->setString(fmt::format("{:.1f}s", time));
}
