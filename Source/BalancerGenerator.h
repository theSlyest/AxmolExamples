#pragma once

#include "axmol.h"

class BalancerGenerator : public ax::DrawNode
{
public:
    static BalancerGenerator* create(const ax::Color4F& color, const std::string& name, float speed);

    virtual bool init(const ax::Color4F& color, const std::string& name, float speed);

    void resetValue(float value);
    float getValue() const { return _bar->getScaleY(); }

    void update(float dt) override;

private:
    static const ax::Size SIZE;
    static const ax::Color4F BG_COLOR;
    static const ax::Color4F BORDER_COLOR;
    static constexpr float BORDER = 4.0f;
    static constexpr float SPACING = 16.0f;
    static constexpr float BAR_PADDING = 4.0f;
    static constexpr float BAR_HEIGHT = 160.0f;
    static constexpr float STEP = 0.25f;
    
    ax::Label* _label = nullptr;
    ax::DrawNode* _bar = nullptr;

    float _speed = 0.1f;
    float _goal = 1.0f;

    void decreaseValue();
    void increaseValue();
};
