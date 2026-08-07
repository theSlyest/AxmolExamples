#pragma once

#include "2d/Scene.h"

class MainMenuScene : public ax::Scene
{
public:
    bool init() override;
    
    CREATE_FUNC(MainMenuScene);

private:
    void startGame() const;
};
