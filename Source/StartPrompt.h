#pragma once

#include "2d/Layer.h"

// The message shown before the first touch: click the surface to send a ripple.
class StartPrompt : public ax::LayerColor
{
public:
    bool init() override;

    CREATE_FUNC(StartPrompt);

    // Center the prompt in an area of the given size.
    void centerIn(const ax::Size& size);

private:
    static constexpr float PADDING = 20.0f;
};
