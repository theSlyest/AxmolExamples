#pragma once

#include <memory>
#include <string>
#include <vector>

#include "2d/Scene.h"
#include "EventListenerTouch.h"

#include "BestScore.h"
#include "LaserBoard.h"
#include "LaserGrid.h"
#include "LaserHud.h"
#include "SolvedPanel.h"

class LaserMirrorScene : public ax::Scene
{
public:
    bool init() override;
    
    // implement the "static create()" method manually
    CREATE_FUNC(LaserMirrorScene);

    void reset();

private:
    // The puzzle, from the top row down: '.' is an empty cell, '#' a wall and 'T' the target.
    static inline const std::vector<std::string> LEVEL {
        "............",
        "............",
        "............",
        ".....#......",
        "..#..T#.....",
        "...#.#......",
        "....#.......",
        "............",
    };

    static constexpr int INITIAL_MIRRORS = 5;
    static constexpr LaserSide EMITTER_SIDE = LaserSide::LEFT;
    static constexpr int EMITTER_POS = 6;

    std::unique_ptr<LaserBoard> _board;
    BestScore _best{ INITIAL_MIRRORS };

    LaserGrid* _grid = nullptr;
    LaserHud* _hud = nullptr;
    SolvedPanel* _panel = nullptr;

    bool _isSolved = false;

    void initGrid(const ax::Size& visibleSize, ax::Vec2 origin);
    void initPanel(const ax::Size& visibleSize, ax::Vec2 origin);
    void initHud(const ax::Size& visibleSize, ax::Vec2 origin);
    void initTouchListener();

    bool onTouchBegan(ax::Touch* touch, ax::Event* event);

    // Show the mirror counts from the board.
    void updateHud() const;

    void showPanel();
};
