#pragma once

#include "axmol.h"

class Helper 
{
public:
    static void drawRoundedRect(ax::DrawNode* drawNode, const ax::Vec2& origin,
        const ax::Vec2& destination, float radius, const ax::Color4F& fillColor,
        const ax::Color4F& borderColor = ax::Color4F(0.0f, 0.0f, 0.0f, 0.0f), float thickness = 1.0f);
};
