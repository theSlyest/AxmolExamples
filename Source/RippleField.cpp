#include "RippleField.h"

#include <cmath>

void RippleField::add(const ax::Vec2& origin)
{
    Ripple ripple;
    ripple.origin = origin;
    _ripples.push_back(ripple);
}

void RippleField::advance(const float dt)
{
    for (auto it = _ripples.begin(); it != _ripples.end(); ) {
        it->elapsed += dt;
        it->radius += SPEED * dt;

        const float lifeRatio = it->elapsed / LIFESPAN;
        if (lifeRatio >= 1.0f) {
            it = _ripples.erase(it);
        }
        else {
            it->amplitude = AMPLITUDE * (1.0f - lifeRatio);
            ++it;
        }
    }
}

float RippleField::heightAt(const float x, const float y) const
{
    float height = 0.0f;

    for (const auto& ripple : _ripples) {
        const float dx = x - ripple.origin.x;
        const float dy = y - ripple.origin.y;
        const float distance = std::sqrt(dx * dx + dy * dy);

        // Only the points near the ripple's front move.
        if (const float distanceFromFront = std::fabs(distance - ripple.radius);
            distanceFromFront < WINDOW && distance > 0.0f) {
            const float wave = std::cos((distance - ripple.radius) * FREQUENCY);
            const float envelope = std::exp(-std::pow(distanceFromFront / WIDTH, 2.0f)); // narrows the ripple
            height += wave * ripple.amplitude * envelope;
        }
    }

    return height;
}
