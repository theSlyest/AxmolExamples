#pragma once

#include "axmol.h"

struct ConfettiParticle {
    ax::Vec2 position;
    ax::Vec2 velocity;
    float rotation{};
    float rotationSpeed{};
    ax::Color4F color;
    float size{};
};

class ConfettiNode : public ax::DrawNode {
public:

    bool init() override;

    // implement the "static create()" method manually
    CREATE_FUNC(ConfettiNode);

    void celebrate();
    void stop();

    void update(float dt) override;

private:
    std::vector<ConfettiParticle> _particles;
    ax::Vec2 _origin;
    ax::Size _visibleSize;

    void reset();
};
