#pragma once

#include "2d/Scene.h"

class DialogueScene2 : public ax::Scene
{
public:
    bool init() override;
    
    CREATE_FUNC(DialogueScene2);

private:
    void showMainMenu() const;

    void onKeyPressed(ax::EventKeyboard::KeyCode key) const;
};
