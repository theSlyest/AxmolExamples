#pragma once


#include "axmol.h"

class Helper {
public:
    static void drawRoundedRect(ax::DrawNode* node, const ax::Vec2& origin,
        const ax::Vec2& dest, float radius, const ax::Color4F& fillColor,
        const ax::Color4F& borderColor = ax::Color4F(), float thickness = 2.0f);

    static void drawDashedRoundedRect(ax::DrawNode* node, const ax::Vec2& origin,
        const ax::Vec2& dest, float radius, float dash, float gap,
        const ax::Color4F& color);

private:
    static void addDashedLinePoints(ax::DrawNode* node,
        const ax::Vec2& from, const ax::Vec2& to, float dash, float gap,
        std::vector<ax::Vec2>& points);
};
