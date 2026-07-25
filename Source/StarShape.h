#pragma once

#include "axmol.h"
#include "Shape.h"

class StarShape : public Shape
{
public:
    virtual bool init(bool isShadow);

    static StarShape* create(bool isShadow = false);

protected:
    static constexpr float Radius = 36.0f;
    static constexpr float InnerRadius = 18.0f;

    ax::Color4F getShapeColor() override;

    void drawType() override;
};
