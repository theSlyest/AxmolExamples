#pragma once

#include <cstddef>
#include <deque>

#include "math/Vec2.h"

// The ripples on the water: how they spread and fade, and the height they give
// to any point of the surface.
//
// This class has no nodes and no drawing, so it can be read and tested on its
// own. Positions are in pixels on the water, with the origin at its corner.
class RippleField
{
public:
    static constexpr float SPEED = 150.0f;      // pixels per second
    static constexpr float AMPLITUDE = 15.0f;   // height of a new ripple
    static constexpr float LIFESPAN = 1.0f;     // seconds before a ripple is gone
    static constexpr float REACH = SPEED * LIFESPAN; // how far a ripple travels

    struct Ripple
    {
        ax::Vec2 origin;
        float radius = 0.0f;
        float amplitude = AMPLITUDE;
        float elapsed = 0.0f;
    };

    // Send a new ripple from `origin`.
    void add(const ax::Vec2& origin);

    // Let every ripple spread and fade, and drop the ones that are gone.
    void advance(float dt);

    // Calm the water: drop every ripple.
    void clear() { _ripples.clear(); }

    [[nodiscard]] bool isEmpty() const { return _ripples.empty(); }
    [[nodiscard]] const std::deque<Ripple>& getRipples() const { return _ripples; }

    // The height of the surface at a point: the heights of all the ripples added up.
    [[nodiscard]] float heightAt(float x, float y) const;

private:
    // The shape of the wave around a ripple's front.
    static constexpr float WINDOW = 120.0f;     // only points this close to the front move
    static constexpr float FREQUENCY = 0.1f;    // oscillation per pixel (in radians)
    static constexpr float WIDTH = 40.0f;       // how fast the wave dies away from the front

    std::deque<Ripple> _ripples;
};
