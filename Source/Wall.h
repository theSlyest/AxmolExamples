#ifndef __LASER_MIRROR_WALL_H__
#define __LASER_MIRROR_WALL_H__

#include "axmol.h"
#include "GridCell.h"

class Wall : public GridCell
{
public:
    static constexpr float WALL_SIZE = 44.0f;
    static const ax::Color4F WALL_COLOR;

    virtual bool init();
    
    // implement the "static create()" method manually
    CREATE_FUNC(Wall);

    virtual void onTouch() override {}

    virtual bool drawBeam(LaserSide from, LaserSide& to) override;
};

#endif // __LASER_MIRROR_WALL_H__
