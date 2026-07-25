#pragma once

#include "axmol.h"
#include "Shape.h"

class CircleShape : public Shape
{
public:
    virtual bool init(bool isShadow);

    static CircleShape* create(bool isShadow = false);

protected:
    static constexpr float Radius = 36.0f;

    ax::Color4F getShapeColor() override;

    void drawType() override;
};
