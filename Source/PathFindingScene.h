#pragma once

#include "axmol.h"
#include "Grid.h"
#include "PathFinder.h"

class PathFindingScene : public ax::Scene
{
public:
    static ax::Scene* createScene();

    bool init() override;
    
    // implement the "static create()" method manually
    CREATE_FUNC(PathFindingScene);

private:
    bool _isModeSearch = false;

    Grid* _grid = nullptr;
    ax::Label* _modeLabel = nullptr;
    ax::Label* _stepLabel = nullptr;
    ax::EventListenerKeyboard* _keyListener = nullptr;
    ax::EventListenerMouse* _mouseListener = nullptr;

    void initGrid();
    void initMode(const ax::Size& visibleSize);
    void initStep(const ax::Size& visibleSize);
    void initKeyboardListener();
    void initMouseListener();

    void setMode(bool isSearch);
    void setStep(int step) const;

    void onKeyPressed(ax::EventKeyboard::KeyCode key);
    bool onMouseUp(const ax::EventMouse *event) const;

    void reset();
    void displayPath(const std::vector<Coords>& path) const;
};
