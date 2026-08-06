#pragma once

#include "axmol.h"

class BalancerChart : public ax::DrawNode
{
public:
    bool init(float tolerance);
    
    // implement the "static create()" method manually
    static BalancerChart* create(float tolerance);

    void update(float dt) override;

    void setSupply(const float supply) { _supply = supply; }

    float getDemand() const { return _demandY / _contentSize.height; }

    void reset(float initialValue);

    void setGraceVisible(const bool visible) const { _graceNode->setVisible(visible); }
    void setGraceTime(float time) const;

private:
    const ax::Color4F BAND_COLOR = ax::Color4F(1.0f, 1.0f, 0.2f, 0.35f);
    const ax::Color4F DEMAND_COLOR = ax::Color4F(1.0f, 1.0f, 0.2f, 1.0f);
    const ax::Color4F SUPPLY_COLOR = ax::Color4F(0.2f, 0.6f, 1.0f, 1.0f);
    const ax::Color4F BG_COLOR = ax::Color4F(0.15f, 0.20f, 0.25f, 1.0f);
    const ax::Color4F BORDER_COLOR = ax::Color4F(0.85f, 0.85f, 0.85f, 1.0f);
    const ax::Color4F LINE_COLOR = ax::Color4F(0.85f, 0.85f, 0.85f, 0.5f);
    const float BORDER = 3.0f;
    const float LINE = 2.0f;
    const float CHART = 5.0f;
    const float SPEED_X = 32.0f;
    const float GRACE_RADIUS = 10.0f;

    const float PERIOD_DELAY = 3.0f;
    const float PERIOD_STEP = 0.25f;
    const float PERIOD_MIN = 6.0f;
    const float PERIOD_START = 12.0f;
    const float MIN_AMP = 0.1f;
    const float MAX_AMP = 0.5f;

    ax::Node* _charts = nullptr;
    ax::Camera* _chartCamera = nullptr;
    ax::DrawNode* _demandChart = nullptr;
    ax::DrawNode* _bandChart = nullptr;
    ax::DrawNode* _supplyChart = nullptr;
    ax::DrawNode* _graceNode = nullptr;
    ax::Label* _graceLbl = nullptr;

    float _toleranceWidth = 0.0f;
    float _demandY = 100.0f;
    float _supply = 0.5f;
    float _supplyY = 100.0f;
    float _xt = 0.0f;

    float _elapsed = 0.0f;
    float _midLine = 0.5f;
    float _amplitude = 0.0f;
    float _period = 0.0f;
    float _periodTimer = 0.0f;
    float _phase = 0.0f;
    int _sinSign = 1;

    void onExtremumReached(bool isPeak);
};
