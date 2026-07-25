#include "ConfettiNode.h"

USING_NS_AX;

bool ConfettiNode::init() 
{
    if ( !DrawNode::init() )
        return false;

    _visibleSize = Director::getInstance()->getVisibleSize();
    _origin = Director::getInstance()->getVisibleOrigin();

    for (int i = 0; i < 100; ++i) {
        ConfettiParticle p;
        _particles.push_back(p);
    }

    setVisible(false);

    return true;
}

void ConfettiNode::reset() 
{
    const float maxVelocityY = -_visibleSize.height - 40.0f;
    const float minVelocityY = maxVelocityY / 2.0f;
    for (auto& p : _particles) {
        p.position = Vec2(_origin.x + rand_0_1() * _visibleSize.width, _origin.y + _visibleSize.height + 20.0f);
        p.velocity = Vec2(rand_minus1_1() + 100.0f, random(maxVelocityY, minVelocityY));
        p.rotation = M_PI * rand_0_1();
        p.rotationSpeed = M_PI * rand_minus1_1();
        p.color = Color4F(rand_0_1(), rand_0_1(), rand_0_1(), 1.0f);
        p.size = random(5.0f, 15.0f);
    }
}

void ConfettiNode::update(const float dt)
{
    clear();
    for (auto& p : _particles) {
        if (p.position.y < _origin.y - 20.0f) {
            continue;
        }

        p.position += p.velocity * dt;
        p.rotation += p.rotationSpeed * dt;

        const float halfSize = p.size / 2.0f;
        const float cosA = cosf(p.rotation);
        const float sinA = sinf(p.rotation);

        const Vec2 vertices[4] = {
            Vec2(-halfSize, -halfSize),
            Vec2(halfSize, -halfSize),
            Vec2(halfSize, halfSize),
            Vec2(halfSize, -halfSize),
        };

        Vec2 points[4];
        for (int i = 0; i < 4; ++i) {
            points[i].x = p.position.x + (vertices[i].x * cosA - vertices[i].y * sinA);
            points[i].y = p.position.y + (vertices[i].x * sinA + vertices[i].y * cosA);
        }

        AXLOGD("Confetti x: {}, y: {}", p.position.x, p.position.y);
        drawSolidPoly(points, 4, p.color);
    }
}

void ConfettiNode::celebrate()
{
    reset();
    setVisible(true);
    scheduleUpdate();
    scheduleOnce([this](float) { stop(); }, 2.0f, "stopCelebrating");
}

void ConfettiNode::stop()
{
    unschedule("stopCelebrating");
    unscheduleUpdate();
    setVisible(false);
}