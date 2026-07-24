#ifndef __MENU_SCENE_H__
#define __MENU_SCENE_H__

#include "axmol.h"

class MenuScene : public ax::Scene
{
public:
    virtual bool init();
    
    // implement the "static create()" method manually
    CREATE_FUNC(MenuScene);

    void toGameScene();

private:
    static constexpr float ButtonFontSize = 48.0f;
    static constexpr float TitleFontSize = 72.0f;
    static constexpr float TitleOffset = 72.0f;
};

#endif // __MENU_SCENE_H__
