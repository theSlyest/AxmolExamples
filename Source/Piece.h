#pragma once

#include "axmol.h"

constexpr float PIECE_RADIUS = 28.0f;
constexpr int PIECE_SEGMENTS = 128;

class Piece : public ax::DrawNode
{
public:
    virtual bool init(bool isBlack);
    
    static Piece* create(bool isBlack = false);

    bool isBlack() const { return _isBlack; }

    void setBlack(bool isBlack
    );

private:
    static const ax::Color4F BgWhite;
    static const ax::Color4F BgBlack;
    static const ax::Color4F BorderWhite;
    static const ax::Color4F BorderBlack;

    bool _isBlack = false;

    void drawEllipse();
};
