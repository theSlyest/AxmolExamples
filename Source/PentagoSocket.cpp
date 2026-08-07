#include "PentagoSocket.h"

USING_NS_AX;

const Color4F PentagoSocket::COLOR(Color4B(0x8, 0x9, 0x11, 0xff));
const Color4F PentagoSocket::RING_COLOR(Color4B(0x43, 0xc6, 0xb8, 0xff));
const Color4F PentagoSocket::MARBLE_BG[2] {
    Color4F(Color4B(0xf4, 0xef, 0xe4, 0xff)),
    Color4F(Color4B(0x10, 0x13, 0x18, 0xff))
};
const Color4F PentagoSocket::MARBLE_BORDER[2] {
    Color4F(Color4B(0x9a, 0x8f, 0x78, 0xff)),
    Color4F(Color4B(0x4a, 0x54, 0x62, 0xff))
};

bool PentagoSocket::init()
{
    if (!DrawNode::init())
        return false;

    constexpr float diameter = 2.0f * RADIUS;
    setContentSize(Vec2(diameter, diameter));
    constexpr int segments = RADIUS * M_PI;
    drawSolidCircle(Vec2(RADIUS, RADIUS), RADIUS, 0.0f, segments, COLOR);

    _marble = DrawNode::create();
    _marble->setContentSize(_contentSize);
    addChild(_marble);

    _ring = DrawNode::create();
    _ring->setContentSize(_contentSize);
    _ring->drawCircle(_contentSize * 0.5f, RADIUS, 0.0f, segments, false, RING_COLOR, OUTLINE);
    _ring->setVisible(false);
    addChild(_ring);
    
    _state = SocketState::EMPTY;;

    return true;
}

void PentagoSocket::setState(const SocketState state)
{
    if (state == _state)
        return;

    _state = state;
    switch (state) {
        case SocketState::EMPTY:
            _marble->clear();
            _ring->setVisible(false);
            break;

        case SocketState::WHITE:
            drawMarble(0);
            break;

        case SocketState::BLACK:
            drawMarble(1);
            break;
    }
}

void PentagoSocket::setBlack(const bool isBlack)
{
    setState(isBlack ? SocketState::BLACK : SocketState::WHITE);
}

void PentagoSocket::drawMarble(const int idx) const
{
    constexpr int segments = MARBLE_RADIUS * M_PI;
    const Vec2 center = _contentSize / 2.0f;
    _marble->clear();
    _marble->drawSolidCircle(center, MARBLE_RADIUS, 0.0f, segments, 1.0f, 1.0f, MARBLE_BG[idx], OUTLINE / 2.0f, MARBLE_BORDER[idx]);
}
