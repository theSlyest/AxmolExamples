#pragma once

#include "axmol.h"
#include "Shape.h"

class OvalShape : public Shape
{
public:
    virtual bool init(bool isShadow);

    static OvalShape* create(bool isShadow = false);

protected:
    static constexpr float Radius = 36.0f;
    static constexpr float SmallRadius = 24.0f;

    ax::Color4F getShapeColor() override;

    void drawType() override;
};
