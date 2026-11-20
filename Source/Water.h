#pragma once

#include "2d/Sprite.h"
#include "RippleField.h"

class Water : public ax::Sprite
{
public:
    static constexpr int GRID_LINES = 35;  // vertices along each side of the mesh
    static constexpr int CELL_SIZE = 28;   // pixels between two vertices
    static constexpr int GRID_SIZE = (GRID_LINES - 1) * CELL_SIZE;

    bool init() override;
    
    // implement the "static create()" method manually
    CREATE_FUNC(Water);

    void update(float dt) override;

    bool onTouch(ax::Touch* touch, ax::Event* event);

    void flatten();

private:
    static const ax::Color4B LineColor;

    RippleField _ripples;
    bool _isFlat = true; // the mesh is at rest, so there is nothing to update

    ax::Texture2D* createSurface();
    ax::PolygonInfo createMesh();

    void updateMesh();

};

