#pragma once

#include "axmol.h"
#include "Shape.h"

class RectangleShape : public Shape
{
public:
    virtual bool init(bool isShadow);

    static RectangleShape* create(bool isShadow = false);

protected:
    static constexpr float Height = 52.0f;
    static constexpr float Width = 92.0f;

    ax::Color4F getShapeColor() override;

    void drawType() override;
};
