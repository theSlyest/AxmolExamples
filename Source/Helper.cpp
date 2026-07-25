#include "Helper.h"

void Helper::drawRoundedRect(ax::DrawNode* drawNode, const ax::Vec2& origin, const ax::Vec2& destination, float radius,
    const ax::Color4F& fillColor, const ax::Color4F& borderColor, const float thickness) {
    // 1. Determine minimum and maximum bounds
    const float minX = std::min(origin.x, destination.x);
    const float maxX = std::max(origin.x, destination.x);
    const float minY = std::min(origin.y, destination.y);
    const float maxY = std::max(origin.y, destination.y);

    // 2. Clamp radius to prevent corner overlapping distortions
    if (const float maxRadius = std::min(maxX - minX, maxY - minY) / 2.0f; radius > maxRadius) {
        radius = maxRadius;
    }

    // 3. Define the center points for all four corner arcs
    const ax::Vec2 centers[4] = {
        ax::Vec2(maxX - radius, maxY - radius), // Top Right
        ax::Vec2(minX + radius, maxY - radius), // Top Left
        ax::Vec2(minX + radius, minY + radius), // Bottom Left
        ax::Vec2(maxX - radius, minY + radius)  // Bottom Right
    };

    // 4. Define starting and ending angles (in radians) for each corner arc
    constexpr float angles[4][2] = {
        {0.0f, M_PI_2},          // Top Right Arc (0 to 90 degrees)
        {M_PI_2, M_PI},          // Top Left Arc (90 to 180 degrees)
        {M_PI, 3.0f * M_PI_2},   // Bottom Left Arc (180 to 270 degrees)
        {3.0f * M_PI_2, M_PI * 2.0f} // Bottom Right Arc (270 to 360 degrees)
    };

    std::vector<ax::Vec2> vertices;

    // 5. Calculate vertices along the perimeter
    for (int i = 0; i < 4; ++i) {
        constexpr int segmentsPerCorner = 8;
        const float startAngle = angles[i][0];
        const float endAngle = angles[i][1];
        const float angleStep = (endAngle - startAngle) / segmentsPerCorner;

        for (int j = 0; j <= segmentsPerCorner; ++j) {
            const float a = startAngle + angleStep * j;
            float x = centers[i].x + radius * cosf(a);
            float y = centers[i].y + radius * sinf(a);
            vertices.emplace_back(x, y);
        }
    }

    // 6. Push the vertices to the DrawNode batch as a filled polygon
    drawNode->drawPolygon(&vertices[0], vertices.size(), fillColor, thickness, borderColor);
}