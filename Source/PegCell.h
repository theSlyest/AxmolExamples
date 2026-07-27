#pragma once

#include "axmol.h"

class PegCell : public ax::DrawNode
{
public:
    static constexpr float SIZE = 60.0f;

    virtual bool init(bool hasPeg);
    
    // implement the "static create()" method manually
    static PegCell* create(bool hasPeg = true);

    void setPeg(bool peg) const;
    bool hasPeg() const { return _peg->isVisible(); }

    void setSelected(bool select) const;
    bool isSelected() const { return _ring->isVisible(); }

private:
    static const ax::Color4F COLOR;
    static const ax::Color4F PEG_COLOR;
    static const ax::Color4F RING_COLOR;
    static constexpr float RADIUS = 24.0f;
    static constexpr float PEG_RADIUS = 18.0f;
    static constexpr float RING_RADIUS = 21.0f;
    static constexpr float RING_WIDTH = 2.0f;

    ax::DrawNode* _peg = nullptr;
    ax::DrawNode* _ring = nullptr;
};
