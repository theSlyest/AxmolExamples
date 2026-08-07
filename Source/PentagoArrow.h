#pragma once

#include "2d/DrawNode.h"

class PentagoArrow : public ax::DrawNode
{
public:
    static constexpr float RADIUS = 12.0f;

    static PentagoArrow* create(int orientation);

    virtual bool init(int orientation);

private:
    static const ax::Color4F COLOR;
    static constexpr float WIDTH = 3.0f;

    ax::DrawNode* createArc() const;
    static ax::DrawNode* createHead();
};
