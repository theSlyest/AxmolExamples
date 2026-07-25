#pragma once

#include "axmol.h"
#include "Shape.h"

class DiamondShape : public Shape
{
public:
    virtual bool init(bool isShadow);

    static DiamondShape* create(bool isShadow = false);

protected:
    static constexpr float Height = 80.0f;
    static constexpr float Width = 72.0f;

    ax::Color4F getShapeColor() override;

    void drawType() override;
};
