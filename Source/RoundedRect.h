#ifndef __DISK_TOWER_ROUNDED_RECT_H__
#define __DISK_TOWER_ROUNDED_RECT_H__

#include "axmol.h"

class RoundedRect : public ax::DrawNode
{
public:
    virtual bool init(float width, float height, float radius, const ax::Color4F& color);
    
    static RoundedRect* create(float width, float height, float radius, const ax::Color4F& color);

private:
};

#endif // __DISK_TOWER_ROUNDED_RECT_H__
