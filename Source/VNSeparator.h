#pragma once

#include "2d/DrawNode.h"

class VNSeparator : public ax::DrawNode
{
public:
    static VNSeparator* create(float width, float height, const ax::Color4F& color = ax::Color4F::WHITE);

    virtual bool init(float width, float height, const ax::Color4F& color);
};
