#pragma once

#include "axmol.h"

class FryingPan : public ax::DrawNode
{
public:
    bool init() override;
    
    // implement the "static create()" method manually
    CREATE_FUNC(FryingPan);

private:
    static constexpr float Radius = 200.0f;
    
};
