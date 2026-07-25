#pragma once

#include "axmol.h"

class Destination : public ax::DrawNode
{
public:
    static const ax::Size SIZE;

    bool init() override;

    CREATE_FUNC(Destination);
};
