#include "Piece.h"

USING_NS_AX;

const Color4F Piece::BgWhite = Color4F(0.96f, 0.95f, 0.91f, 1.0f);
const Color4F Piece::BgBlack = Color4F(0.08f, 0.02f, 0.03f, 1.0f);
const Color4F Piece::BorderWhite = Color4F(0.15f, 0.18f, 0.23f, 1.0f);
const Color4F Piece::BorderBlack = Color4F(0.83f, 0.87f, 0.92f, 1.0f);

Piece* Piece::create(const bool isBlack)
{
    if (Piece* pRet = new (std::nothrow) Piece(); pRet && pRet->init(isBlack))
    {
        pRet->autorelease();
        return pRet;
    }
    else
    {
        delete pRet;
        pRet = nullptr;
        return nullptr;
    }
}

// on "init" you need to initialize your instance
bool Piece::init(const bool isBlack)
{
    if ( !DrawNode::init() )
    {
        return false;
    }

    constexpr float diameter = PIECE_RADIUS * 2.0f;
    setContentSize(Size(diameter, diameter));

    setBlack(isBlack);

    return true;
}

void Piece::drawEllipse()
{
    // Define ellipse properties
    Vec2 center = Vec2(0.75f * PIECE_RADIUS, 1.5f * PIECE_RADIUS); // Center position
    float radiusX = PIECE_RADIUS / 4.0f;    // Horizontal radius
    float radiusY = PIECE_RADIUS / 6.0f;    // Vertical radius
    int segments = PIECE_SEGMENTS;                      // Smoothness of the curve
    float angle = 0.0f;                     // Starting angle

    // Array to store the calculated points
    std::vector<Vec2> vertices;

    // Calculate points along the ellipse perimeter
    for (int i = 0; i < segments; ++i)
    {
        float x = center.x + radiusX * cosf(angle);
        float y = center.y + radiusY * sinf(angle);
        
        vertices.emplace_back(x, y);
        
        // Increment angle based on number of segments
        angle += 360.0f / segments;
    }

    // Draw the ellipse using the vertices
    drawPolygon(&vertices[0], segments, Color4F::WHITE, 0.0f, Color4F(0.0f, 0.0f, 0.0f, 0.0f));
}

void Piece::setBlack(const bool isBlack)
{
    _isBlack = isBlack;

    clear();
 
    Color4F bgColor;
    Color4F borderColor;
    if (isBlack) {
        bgColor = BgBlack;
        borderColor = BorderBlack;
    }
    else {
        bgColor = BgWhite;
        borderColor = BorderWhite;
    }

    constexpr Vec2 center(PIECE_RADIUS, PIECE_RADIUS);

    drawSolidCircle(center + Vec2(4.0f, -4.0f), PIECE_RADIUS, 0.0f, PIECE_SEGMENTS, Color4F::BLACK);
    drawSolidCircle(center, PIECE_RADIUS, 0.0f, PIECE_SEGMENTS, borderColor);
    drawSolidCircle(center, PIECE_RADIUS - 3.0f, 0.0f, PIECE_SEGMENTS, bgColor);
    drawCircle(Vec2(PIECE_RADIUS, PIECE_RADIUS), PIECE_RADIUS - 7.0f, 0.0f, PIECE_SEGMENTS, false, Color4F::WHITE, 1.0f);
    drawEllipse();

}