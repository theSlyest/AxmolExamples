#pragma once

#include "2d/DrawNode.h"
#include "PentagoSocket.h"

class PentagoBlock : public ax::DrawNode
{
public:
    static const std::string TURNED_EVENT;

    static constexpr float SIZE = 228.0f;

    static PentagoBlock* create(bool arrowsUp = false);

    static void setTurning(const bool isTurning) { _IsTurning = isTurning; }
    static bool isTurning() { return _IsTurning; }

    static void setEnabled(const bool enabled) { _IsEnabled = enabled; }
    static bool isEnabled() { return _IsEnabled; }

    virtual bool init(bool arrowsUp);

    bool onTouch(const ax::Vec2& location, bool isBlack = false) const;

    void reset() const;

    std::vector<PentagoSocket*> at(int col) { return _sockets[col]; }
    
private:
    static const ax::Color4F COLOR;

    static constexpr float RADIUS = 16.0f;
    static constexpr float ARROW_OFFSET = 24.0f;
    static constexpr float ARROW_PADDING = 156.0f;
    static constexpr float OFFSET = 26.0f;
    static constexpr float OFFSET_D = 8.0f;
    static constexpr float SPACING = 62.0f;

    static bool _IsTurning;
    static bool _IsEnabled;

    std::vector<std::vector<PentagoSocket*>> _sockets;
    ax::DrawNode* _socketNode = nullptr;

    bool _arrowsUp = false;

    void drawBackground();
    void drawSockets();

    void initArrows(bool arrowsUp);

    void transpose() const;
    void rotateLeft() const;
    void rotateRight() const;

    void leftArrowCallback() const;
    void rightArrowCallback() const;

    void dispatchTurnedEvent() const;
};
