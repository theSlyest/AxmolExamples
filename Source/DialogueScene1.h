#pragma once

#include "2d/Scene.h"

class DialogueScene1 : public ax::Scene
{
public:
    bool init() override;
    
    CREATE_FUNC(DialogueScene1);

private:
    void showDialogue2() const;

    void onKeyPressed(ax::EventKeyboard::KeyCode key) const;
};
