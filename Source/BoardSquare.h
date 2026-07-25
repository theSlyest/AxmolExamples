#pragma once

#include "axmol.h"

constexpr float SQUARE_SIDE = 72.0f;

class BoardSquare : public ax::DrawNode
{
public:
    virtual bool init(bool isBlack);
    
    static BoardSquare* create(bool isBlack = false);

    bool isBlack() const { return _isBlack; }

    bool isLegal() const { return _legalNode->isVisible(); }
    void setLegal(const bool isLegal) const { _legalNode->setVisible(isLegal); }

    void setHovered(bool hovered);

    void onMouseOver();
    void onMouseOut();

private:
    static const ax::Color4F White;
    static const ax::Color4F Black;
    static const ax::Color4F Light;
    static const ax::Color4F Dark;

    ax::DrawNode* _legalNode = nullptr;
    ax::DrawNode* _hoverNode = nullptr;
    
    bool _isBlack = false;
    bool _isHovering = false;

    void initLegalNode();
    void initHoverNode();
};
