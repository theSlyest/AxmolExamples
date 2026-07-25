#pragma once

#include "axmol.h"
#include "Shape.h"

class TriangleShape : public Shape
{
public:
    virtual bool init(bool isShadow);

    static TriangleShape* create(bool isShadow = false);

protected:
    static constexpr float Side = 76.0f;

    ax::Color4F getShapeColor() override;

    void drawType() override;
};
