#pragma once

#include "axmol.h"
#include "Shape.h"

class HeartShape : public Shape
{
public:
    virtual bool init(bool isShadow);

    static HeartShape* create(bool isShadow = false);

protected:
    static constexpr float Height = 72.0f;

    ax::Color4F getShapeColor() override;

    void drawType() override;
};
