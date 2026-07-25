#ifndef __LASER_MIRROR_TARGET_H__
#define __LASER_MIRROR_TARGET_H__

#include "axmol.h"
#include "GridCell.h"

class Target : public GridCell
{
public:
    static const ax::Color4F COLOR;
    static const ax::Color4F LIGHT_COLOR;

    virtual bool init();
    
    // implement the "static create()" method manually
    CREATE_FUNC(Target);

    void setLit(bool isLit) { _lightNode->setVisible(isLit); }
    bool isLit() const { return _lightNode->isVisible(); }

    virtual void onTouch() override {}

    virtual bool drawBeam(LaserSide from, LaserSide& to) override;
    virtual void clearBeam() override;

private:
    static constexpr float INNER_RADIUS = 6.0f;
    static constexpr float OUTER_RADIUS = 15.0f;
    static constexpr float OUTER_WIDTH = 6.0f;
    static constexpr float LIGHT_RADIUS = 21.0f;

    ax::DrawNode* _lightNode;
};

#endif // __LASER_MIRROR_TARGET_H__
