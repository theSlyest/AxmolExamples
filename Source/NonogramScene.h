#ifndef __NONOGRAM_SCENE_H__
#define __NONOGRAM_SCENE_H__

#include "2d/Scene.h"
#include "EventListenerKeyboard.h"
#include "EventListenerMouse.h"
#include "2d/Label.h"
#include "2d/Layer.h"
#include "Cell.h"

class NonogramScene : public ax::Scene
{
public:
    static ax::Scene* createScene();

    virtual bool init();
    
    // implement the "static create()" method manually
    CREATE_FUNC(NonogramScene);

private:
    static constexpr int GRID_ROWS = 10;
    static constexpr int GRID_COLS = 10;

    std::vector<std::vector<Cell*>> _grid;
    std::vector<std::vector<bool>> _solution;
    std::vector<ax::Label*> _rowHints;
    std::vector<ax::Label*> _colHints;

    ax::EventListenerKeyboard* _keyboardListener = nullptr;
    ax::EventListenerMouse* _mouseListener = nullptr;
    ax::LayerColor* _winPanel = nullptr;

    void initGrid(const ax::Vec2& origin, const ax::Size& visibleSize);
    void initWinPanel(const ax::Vec2& origin, const ax::Size& visibleSize);
    void initKeyboardListener();
    void initMouseListener();

    bool onMouseDown(const ax::EventMouse* event);

    void resetGrid() const;

    void restart() const;

    bool checkSolved();
};

#endif // __NONOGRAM_SCENE_H__
