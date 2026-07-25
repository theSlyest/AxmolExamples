#ifndef __RIPPLE_CONTACT_H__
#define __RIPPLE_CONTACT_H__

#include "axmol.h"

constexpr float CON_RADIUS = 7.0f;
constexpr float CON_SPEED = 16.0f;

class Contact : public ax::DrawNode
{
public:
    virtual bool init() override;

    CREATE_FUNC(Contact);

    void setDirection(ax::Vec2 direction) { _direction = direction; }
    ax::Vec2 getDirection() const { return _direction; }

    void setRandomDirection();

private:
    ax::Vec2 _direction;
};

#endif // __RIPPLE_CONTACT_H__