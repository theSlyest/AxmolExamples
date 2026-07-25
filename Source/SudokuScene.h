#ifndef __SUDOKU_SCENE_H__
#define __SUDOKU_SCENE_H__

#include "axmol.h"
#include "SudokuGrid.h"

class SudokuScene : public ax::Scene
{
public:    
    static ax::Scene* createScene();

    virtual bool init();

    // implement the "static create()" method manually
    CREATE_FUNC(SudokuScene);

private:
    const ax::Color4B StatusColor = ax::Color4B(207, 212, 224, 255);

    SudokuGrid* _grid;
    const std::vector<std::vector<char>> CLUES {
        { 0, '2', '1', 0, 0, 0, 0, '7', 0 },
        { '9', '3', 0, 0, '4', 0, 0, 0, 0 },
        { 0, '7', 0, 0, 0, 0, '3', '4', 0 },
        { '2', 0, 0, '9', 0, '3', 0, '5', 0 },
        { '3', 0, '5', 0, 0, 0, '9', 0, '6' },
        { 0, '6', 0, '8', 0, '5', 0, 0, '1' },
        { 0, '9', '3', 0, 0, 0, 0, '6', 0 },
        { 0, 0, 0, 0, '9', 0, 0, '1', '8' },
        { 0, '5', 0, 0, 0, 0, '7', '9', 0 }
    };

    ax::Label* _status;
    ax::LayerColor* _solved;

    ax::EventListenerMouse* _mouseListener;
    ax::EventListenerKeyboard* _keyboardListener;

    void initTitle(const ax::Size &visibleSize, const ax::Vec2 &origin);
    void initGrid(const ax::Size& visibleSize, const ax::Vec2& origin);
    void initStatus(const ax::Size& visibleSize, const ax::Vec2& origin);
    void initHelp(const ax::Size& visibleSize, const ax::Vec2& origin);
    void initSolved();

    void initMouseListener();
    bool onMouseDown(ax::EventMouse *event);

    void initKeyboardListener();
    void onKeyPressed(ax::EventKeyboard::KeyCode key);

    void setStatus(const int leftCount);

    void reset();

    void showSolved();
};

#endif // __SUDOKU_SCENE_H__
