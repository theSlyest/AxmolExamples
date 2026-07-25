#pragma once

#include "axmol.h"
#include "Shape.h"

class SquareShape : public Shape
{
public:
    virtual bool init(bool isShadow);

    static SquareShape* create(bool isShadow = false);

protected:
    static constexpr float Side = 72.0f;

    ax::Color4F getShapeColor() override;

    void drawType() override;
};
