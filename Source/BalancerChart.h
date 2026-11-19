#pragma once

#include "2d/DrawNode.h"
#include "2d/Label.h"

#include <deque>

class BalancerChart : public ax::DrawNode
{
public:
    bool init(float tolerance);
    
    // implement the "static create()" method manually
    static BalancerChart* create(float tolerance);

    // `demand` and `supply` are normalized between 0 and 1.
    void update(float dt, float demand, float supply);

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
    const float SPEED_X = 32.0f;          // scrolling speed, in pixels per second
    const float SAMPLE_INTERVAL = 1.0f / 30.0f; // the history is sampled 30 times per second
    const float HEAD_X = 0.96f;           // the newest sample stays at this fraction of the width
    const float GRACE_RADIUS = 10.0f;

    // One point of the demand and supply curves, in pixels.
    struct Sample
    {
        float demand;
        float supply;
    };

    ax::Node* _charts = nullptr;
    ax::DrawNode* _demandChart = nullptr;
    ax::DrawNode* _bandChart = nullptr;
    ax::DrawNode* _supplyChart = nullptr;
    ax::DrawNode* _graceNode = nullptr;
    ax::Label* _graceLbl = nullptr;

    float _toleranceWidth = 0.0f;
    float _demandY = 100.0f;
    float _supplyY = 100.0f;
    // Bounded history: the oldest samples are dropped once they scroll out of the chart.
    std::deque<Sample> _history;
    size_t _capacity = 0;
    float _sampleTimer = 0.0f;
    int _sampleIndex = 0; // samples taken since the last reset, minus one
    float _headX = 0.0f;  // x position of the newest sample

    static void addLegendLabel(ax::Node* legend, const std::string& text, float x);
    void addSample();
    void redraw() const;
};
