#include "Helper.h"

USING_NS_AX;

void Helper::drawRoundedRect(DrawNode* node, const Vec2& origin, const Vec2& dest,
    float radius, const Color4F& fillColor, const Color4F& borderColor, const float thickness)
{
    // Determine bounds
    const float minX = std::min(origin.x, dest.x);
    const float maxX = std::max(origin.x, dest.x);
    const float minY = std::min(origin.y, dest.y);
    const float maxY = std::max(origin.y, dest.y);

    // Clamp radius so it doesn't exceed half the width or height
    if (const float maxRadius = std::min((maxX - minX) / 2.0f, (maxY - minY) / 2.0f); radius > maxRadius) {
        radius = maxRadius;
    }

    std::vector<Vec2> vertices;
    const int segments = radius * M_PI_2; // Precision per corner. Increase for smoother corners.
    const float angleStep = M_PI / (2.0f * segments);

    // 1. Bottom-Right Corner
    const Vec2 centerBR(maxX - radius, minY + radius);
    for (int i = 0; i <= segments; ++i) {
        const float angle = -M_PI_2 + i * angleStep;
        vertices.push_back(centerBR + Vec2(cos(angle) * radius, sin(angle) * radius));
    }

    // 2. Top-Right Corner
    const Vec2 centerTR(maxX - radius, maxY - radius);
    for (int i = 0; i <= segments; ++i) {
        const float angle = 0.0f + i * angleStep;
        vertices.push_back(centerTR + Vec2(cos(angle) * radius, sin(angle) * radius));
    }

    // 3. Top-Left Corner
    const Vec2 centerTL(minX + radius, maxY - radius);
    for (int i = 0; i <= segments; ++i) {
        const float angle = M_PI_2 + i * angleStep;
        vertices.push_back(centerTL + Vec2(cos(angle) * radius, sin(angle) * radius));
    }

    // 4. Bottom-Left Corner
    const Vec2 centerBL(minX + radius, minY + radius);
    for (int i = 0; i <= segments; ++i) {
        const float angle = M_PI + i * angleStep;
        vertices.push_back(centerBL + Vec2(cos(angle) * radius, sin(angle) * radius));
    }

    // Pass the calculated vertices to DrawNode
    node->drawPolygon(&vertices[0], vertices.size(), fillColor, thickness, borderColor);
}

void  Helper::addDashedLinePoints(DrawNode* node, const Vec2& from, const Vec2& to, const float dash,
    const float gap, std::vector<Vec2>& points)
{
    const float dx = to.x - from.x;
    const float dy = to.y - from.y;
    const float length = std::sqrt(dx * dx + dy * dy);
    
    if (length == 0) return;

    const float unitX = dx / length;
    const float unitY = dy / length;

    float distance = 0;
    bool drawing = true;

    Vec2 current = from;

    while (distance < length) {
        float step = drawing ? dash : gap;
        if (distance + step > length) {
            step = length - distance;
        }

        if (drawing) {
            points.push_back(current);
            points.push_back(current + Vec2(unitX * step, unitY * step));
        }

        current += Vec2(unitX * step, unitY * step);
        distance += step;
        drawing = !drawing;
    }
}

void Helper::drawDashedRoundedRect(DrawNode* node, const Vec2& origin, const Vec2& dest,
    float radius, const float dash, const float gap, const Color4F& color)
{
    const float minX = std::min(origin.x, dest.x);
    const float maxX = std::max(origin.x, dest.x);
    const float minY = std::min(origin.y, dest.y);
    const float maxY = std::max(origin.y, dest.y);

    const float width = maxX - minX;
    const float height = maxY - minY;

    // Clamp radius so it doesn't exceed half the width or height
    radius = std::min(radius, std::min(width / 2.0f, height / 2.0f));

    std::vector<Vec2> points;
    constexpr int segments = 4; // Curve segments per corner

    // 1. Bottom Edge (Dashed)
    addDashedLinePoints(node, Vec2(minX + radius, minY), Vec2(maxX - radius, minY), dash, gap, points);

    // 2. Bottom Right Corner
    const Vec2 brCenter(maxX - radius, minY + radius);
    for (int i = 0; i <= segments; ++i) {
        const float angle = -M_PI_2 + (M_PI_2 * i / segments); // From -90 to 0 degrees
        points.push_back(brCenter + Vec2(radius * cos(angle), radius * sin(angle)));
    }

    // 3. Right Edge (Dashed)
    addDashedLinePoints(node, Vec2(maxX, minY + radius), Vec2(maxX, maxY - radius), dash, gap, points);

    // 4. Top Right Corner
    const Vec2 trCenter(maxX - radius, maxY - radius);
    for (int i = 0; i <= segments; ++i) {
        const float angle = M_PI_2 * i / segments; // From 0 to 90 degrees
        points.push_back(trCenter + Vec2(radius * cos(angle), radius * sin(angle)));
    }

    // 5. Top Edge (Dashed)
    addDashedLinePoints(node, Vec2(maxX - radius, maxY), Vec2(minX + radius, maxY), dash, gap, points);

    // 6. Top Left Corner
    const Vec2 tlCenter(minX + radius, maxY - radius);
    for (int i = 0; i <= segments; ++i) {
        const float angle = M_PI_2 + (M_PI_2 * i / segments); // From 90 to 180 degrees
        points.push_back(tlCenter + Vec2(radius * cos(angle), radius * sin(angle)));
    }

    // 7. Left Edge (Dashed)
    addDashedLinePoints(node, Vec2(minX, maxY - radius), Vec2(minX, minY + radius), dash, gap, points);

    // 8. Bottom Left Corner
    const Vec2 blCenter(minX + radius, minY + radius);
    for (int i = 0; i <= segments; ++i) {
        const float angle = M_PI + (M_PI_2 * i / segments); // From 180 to 270 degrees
        points.push_back(blCenter + Vec2(radius * cos(angle), radius * sin(angle)));
    }

    // Draw the segments to the DrawNode (pass pairs of points)
    for (size_t i = 0; i < points.size(); i += 2) {
        if (i + 1 < points.size()) {
            node->drawSegment(points[i], points[i+1], 2.0f, color);
        }
    }
}