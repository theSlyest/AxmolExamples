#ifndef __RIPPLE_WATER_H__
#define __RIPPLE_WATER_H__

#include "axmol.h"

constexpr int GRID_LINES = 35;
constexpr int CELL_SIZE = 28;
constexpr int GRID_SIZE = (GRID_LINES - 1) * CELL_SIZE;

constexpr float RIPPLE_SPEED = 150.0f;
constexpr float RIPPLE_AMPLITUDE = 15.0f;
constexpr float RIPPLE_LIFESPAN = 1.0f;
constexpr float RIPPLE_REACH = 150.0f;

struct Ripple {
    ax::Vec2 origin;
    float radius = 0.0f;
    float amplitude = RIPPLE_AMPLITUDE;
    float elapsed = 0.0f;

    Ripple(ax::Vec2 location) { origin = location; }
};

class Water : public ax::Sprite
{
public:
    virtual bool init() override;
    
    // implement the "static create()" method manually
    CREATE_FUNC(Water);

    virtual void update(float dt) override;

    bool onTouch(ax::Touch* touch, ax::Event* event);

    void flatten();

private:
    static const ax::Color4B LineColor;

    std::deque<Ripple> _ripples;

    ax::Texture2D* createSurface();
    ax::PolygonInfo createMesh();

    void updateRipples(float dt);
    void updateMesh();

};

#endif // __RIPPLE_WATER_H__
