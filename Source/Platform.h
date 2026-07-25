#pragma once

#include "axmol.h"

class Platform : public ax::Sprite
{
public:

    virtual bool init(float width, float height);

    static Platform* create(float width, float height = 20.0f);
};
