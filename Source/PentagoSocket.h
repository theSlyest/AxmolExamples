#pragma once

#include "2d/DrawNode.h"

enum class SocketState { EMPTY, WHITE, BLACK };

class PentagoSocket : public ax::DrawNode
{
public:
    static constexpr float RADIUS = 26.0f;

    bool init() override;
    
    // implement the "static create()" method manually
    CREATE_FUNC(PentagoSocket);

    void setState(SocketState state);
    SocketState getState() const { return _state; }

    void setBlack(bool isBlack);
    bool isEmpty() const { return _state == SocketState::EMPTY; }
    bool isBlack() const { return _state == SocketState::BLACK; }

    void setRing(const bool ring) const { _ring->setVisible(ring); }

private:
    static const ax::Color4F COLOR;
    static const ax::Color4F RING_COLOR;
    static const ax::Color4F MARBLE_BG[2];
    static const ax::Color4F MARBLE_BORDER[2];
    static constexpr float MARBLE_RADIUS = 24.0f;
    static constexpr float OUTLINE = 3.0f;
    
    ax::DrawNode* _marble = nullptr;
    ax::DrawNode* _ring   = nullptr;

    SocketState _state = SocketState::EMPTY;

    void drawMarble(int idx) const;
};
