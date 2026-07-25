#ifndef __LASER_MIRROR_EMITTER_H__
#define __LASER_MIRROR_EMITTER_H__

#include "axmol.h"

class Emitter : public ax::DrawNode
{
public:
    static constexpr float LIGHT_RADIUS = 6.0f;
    static const ax::Size SIZE;
    static const ax::Color4F COLOR;
    static const ax::Color4F LIGHT_COLOR;

    virtual bool init();
    
    // implement the "static create()" method manually
    CREATE_FUNC(Emitter);
};

#endif // __LASER_MIRROR_EMITTER_H__
