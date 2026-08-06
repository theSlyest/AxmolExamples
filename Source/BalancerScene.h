#pragma once

#include "axmol.h"
#include "BalancerValue.h"
#include "BalancerGenerator.h"
#include "BalancerChart.h"

class BalancerScene : public ax::Scene
{
public:
    static ax::Scene* createScene();

    bool init() override;
    
    // implement the "static create()" method manually
    CREATE_FUNC(BalancerScene);

    void update(float dt) override;

private:
    const float UI_OFFSET = 32.0f;
    
    const int STRIKES = 3;
    const int GENS = 3;
    const float GEN_RATES[3] = { 0.5f, 0.25f, 0.125f };
    const std::string GEN_NAMES[3] = { "FAST", "MEDIUM", "SLOW" };
    const ax::Color4F GEN_COLORS[3] = {
        ax::Color4F(0.4f, 1.0f, 0.4f, 1.0f),
        ax::Color4F(0.2f, 0.6f, 1.0f, 1.0f),
        ax::Color4F(1.0f, 0.2f, 1.0f, 1.0f)
    };
    const float GEN_MAX = 1.0f;
    const float GEN_MIN = 0.0f;
    const float GEN_START = 0.5f;
    const float TOLERANCE = 0.1f;
    const float GRACE_PERIOD = 4.0f;
    const char* BEST_KEY = "Best";

    int _strikes = 0;
    float _downTime = 0.0f;
    float _upTime = 0.0f;
    int _best = 0;

    float _demandGoal = 0.0f;
    float _demandAccel = 0.0f;
    bool _demandIncrease = false;
    float _demandFactor = 0.0f;

    BalancerValue* _upTimeUi = nullptr;
    BalancerValue* _strikesUi = nullptr;
    BalancerValue* _bestUi = nullptr;

    std::vector<BalancerGenerator*> _gens;
    BalancerGenerator* _genMedium = nullptr;
    BalancerGenerator* _genSlow = nullptr;

    BalancerChart* _chart = nullptr;

    ax::DrawNode* _endLayer = nullptr;
    ax::Label* _upTimeLbl = nullptr;
    ax::Label* _bestLbl = nullptr;
    ax::EventListenerTouchOneByOne* _touchListener = nullptr;

    void setUpTime(float upTime);
    void setStrikes(int strikes);
    void setBest(int best);

    void initGenerators(const ax::Size& size, const ax::Vec2& origin);
    void initEndLayer(const ax::Size& size, const ax::Vec2& origin);

    void gameOver();
    void reset();
};
