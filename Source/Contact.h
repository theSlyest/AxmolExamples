#pragma once

#include "2d/DrawNode.h"
#include "TankGame.h"

// How a contact looks: an orange disc. The game moves it by setting its position.
class Contact : public ax::DrawNode
{
public:
    static constexpr float RADIUS = TankGame::CONTACT_RADIUS;

    bool init() override;

    CREATE_FUNC(Contact);
};
