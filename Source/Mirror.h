#ifndef __LASER_MIRROR_MIRROR_H__
#define __LASER_MIRROR_MIRROR_H__

#include "axmol.h"
#include "GridCell.h"

class Mirror : public GridCell
{
public:
    static constexpr float MIRROR_SIZE = 40.0f;
    static const ax::Color4F MIRROR_COLOR;

    virtual bool init();
    
    CREATE_FUNC(Mirror);

    virtual void onTouch() override;

    virtual bool drawBeam(LaserSide from, LaserSide& to) override;

protected:
    static constexpr float WIDTH = 6.0f;
    ax::DrawNode* _mirrorNode;
    bool _isTopLeft; // is Top-Left to Bottom-Right

    void drawMirror();

    bool drawReflectedBeam(LaserSide from, LaserSide& to);
};

#endif // __LASER_MIRROR_MIRROR_H__
