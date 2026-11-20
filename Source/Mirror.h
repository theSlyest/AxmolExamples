#pragma once

#include "GridCell.h"

class Mirror : public GridCell
{
public:
    static constexpr float MIRROR_SIZE = 40.0f;
    static const ax::Color4F MIRROR_COLOR;

    bool init() override;
    
    CREATE_FUNC(Mirror);

    void onTouch() override;

    bool drawBeam(LaserSide from, LaserSide& to) override;

protected:
    static constexpr float WIDTH = 6.0f;
    ax::DrawNode* _mirrorNode = nullptr;
    bool _isTopLeft = false; // is Top-Left to Bottom-Right

    void drawMirror() const;

    bool drawReflectedBeam(LaserSide from, LaserSide& to) const;
};
