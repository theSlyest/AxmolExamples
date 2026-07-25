#pragma once

#include "axmol.h"

class Goal : public ax::Sprite
{
public:
    virtual bool init(bool isCheckbox);

    static Goal* create(bool isCheckbox = false);
};
