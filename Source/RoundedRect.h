#pragma once

#include "axmol.h"

class RoundedRect : public ax::DrawNode
{
public:
    static RoundedRect* create(const ax::Size& size, float radius, const ax::Color4F& fillColor, const ax::Color4F& borderColor = ax::Color4F(), float thickness = 1.0f);

    virtual bool init(const ax::Size& size, float radius, const ax::Color4F& fillColor, const ax::Color4F& borderColor, float thickness);

private:
    static void addCornerPoints(std::vector<ax::Vec2>& points, const ax::Vec2& center, float radius, int segments, float startAngle);
};
