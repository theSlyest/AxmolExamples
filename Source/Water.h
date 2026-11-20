#pragma once

#include <deque>

#include "2d/Sprite.h"

class Water : public ax::Sprite
{
public:
    static constexpr int GRID_LINES = 35;  // vertices along each side of the mesh
    static constexpr int CELL_SIZE = 28;   // pixels between two vertices
    static constexpr int GRID_SIZE = (GRID_LINES - 1) * CELL_SIZE;

    static constexpr float RIPPLE_SPEED = 150.0f;     // pixels per second
    static constexpr float RIPPLE_AMPLITUDE = 15.0f;  // height of a new ripple
    static constexpr float RIPPLE_LIFESPAN = 1.0f;    // seconds
    static constexpr float RIPPLE_REACH = RIPPLE_SPEED * RIPPLE_LIFESPAN; // how far a ripple travels

    bool init() override;
    
    // implement the "static create()" method manually
    CREATE_FUNC(Water);

    void update(float dt) override;

    bool onTouch(ax::Touch* touch, ax::Event* event);

    void flatten();

private:
    // The shape of the wave around a ripple's front.
    static constexpr float WAVE_WINDOW = 120.0f;    // only points this close to the front move
    static constexpr float WAVE_FREQUENCY = 0.1f;   // oscillations per pixel (in radians)
    static constexpr float WAVE_WIDTH = 40.0f;      // how fast the wave dies away from the front

    struct Ripple {
        ax::Vec2 origin;
        float radius = 0.0f;
        float amplitude = RIPPLE_AMPLITUDE;
        float elapsed = 0.0f;

        explicit Ripple(const ax::Vec2& location) : origin(location) {}
    };

    static const ax::Color4B LineColor;

    std::deque<Ripple> _ripples;

    ax::Texture2D* createSurface();
    ax::PolygonInfo createMesh();

    void updateRipples(float dt);
    void updateMesh();

};

