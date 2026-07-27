#pragma once

#include "axmol.h"
#include "RushHourGrid.h"
#include "RushHourVehicle.h"

class RushHourScene : public ax::Scene
{
public:
    static ax::Scene* createScene();

    bool init() override;
    
    // implement the "static create()" method manually
    CREATE_FUNC(RushHourScene);

private:
    const float UI_OFFSET = 48.0f;
    static const std::vector<ax::Color4F> V_COLORS;
    static const std::vector<GridCell> V_POS;
    const GridCell EXIT = { 6, 3 };

    int _moves = 0;
    ax::Label* _movesLbl = nullptr;
    ax::Label* _endMovesLbl = nullptr;
    ax::LayerColor* _endLayer = nullptr;

    RushHourGrid* _grid = nullptr;
    std::vector<RushHourVehicle*> _vehicles;
    RushHourVehicle* _dragged = nullptr;
    ax::Vec2 _dragPos;
    ax::Vec2 _dragPosMin;
    ax::Vec2 _dragPosMax;
    GridCell _dragCell;

    ax::EventListenerTouchOneByOne* _touchListener = nullptr;
    ax::EventListenerKeyboard* _keyboardListener = nullptr;

    void initMoves(const ax::Vec2& origin, const ax::Size& size);
    void initGrid(const ax::Vec2& origin, const ax::Size& size);
    void initEndLayer(const ax::Vec2& origin, const ax::Size& size);
    void initVehicles();
    void initTouchListener();
    void initKeyboardListener();

    void setMoves(int moves);

    void resetGrid();
    void reset();

    void positionVehicle(RushHourVehicle* vehicle, const GridCell& cell) const;

    bool onTouchBegan(ax::Touch* touch, ax::Event* event);
    void onTouchMoved(ax::Touch* touch, ax::Event* event);
    void onTouchEnded(ax::Touch* touch, ax::Event* event);
    void onKeyPressed(ax::EventKeyboard::KeyCode key);

    void showEndLayer() const;
};
