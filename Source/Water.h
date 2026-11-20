#pragma once

#include <optional>

#include "2d/Sprite.h"
#include "RippleField.h"

namespace ax { class Touch; }

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

    // Where a touch landed on the water, or nothing if it landed elsewhere.
    [[nodiscard]] std::optional<ax::Vec2> locate(ax::Touch* touch) const;

    // Send a ripple from a point of the water.
    void sendRipple(const ax::Vec2& position);

    void flatten();

private:
    static const ax::Color4B LineColor;

    RippleField _ripples;
    bool _isFlat = true; // the mesh is at rest, so there is nothing to update

    ax::Texture2D* createSurface();
    ax::PolygonInfo createMesh();

    void updateMesh();

};

