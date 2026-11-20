#pragma once

#include "2d/Scene.h"
#include "EventListenerTouch.h"

#include "HudValue.h"
#include "LaserGrid.h"
#include "SolvedPanel.h"

class LaserMirrorScene : public ax::Scene
{
public:
    bool init() override;
    
    // implement the "static create()" method manually
    CREATE_FUNC(LaserMirrorScene);

    void reset();

private:
    static inline const std::vector<std::vector<CellState>> INITIAL_GRID {
        { CellState::EMPTY, CellState::EMPTY, CellState::EMPTY, CellState::EMPTY, CellState::EMPTY, CellState::EMPTY, CellState::EMPTY, CellState::EMPTY, CellState::EMPTY, CellState::EMPTY, CellState::EMPTY, CellState::EMPTY },
        { CellState::EMPTY, CellState::EMPTY, CellState::EMPTY, CellState::EMPTY, CellState::WALL, CellState::EMPTY, CellState::EMPTY, CellState::EMPTY, CellState::EMPTY, CellState::EMPTY, CellState::EMPTY, CellState::EMPTY },
        { CellState::EMPTY, CellState::EMPTY, CellState::EMPTY, CellState::WALL, CellState::EMPTY, CellState::WALL, CellState::EMPTY, CellState::EMPTY, CellState::EMPTY, CellState::EMPTY, CellState::EMPTY, CellState::EMPTY },
        { CellState::EMPTY, CellState::EMPTY, CellState::WALL, CellState::EMPTY, CellState::EMPTY, CellState::TARGET, CellState::WALL, CellState::EMPTY, CellState::EMPTY, CellState::EMPTY, CellState::EMPTY, CellState::EMPTY },
        { CellState::EMPTY, CellState::EMPTY, CellState::EMPTY, CellState::EMPTY, CellState::EMPTY, CellState::WALL, CellState::EMPTY, CellState::EMPTY, CellState::EMPTY, CellState::EMPTY, CellState::EMPTY, CellState::EMPTY },
        { CellState::EMPTY, CellState::EMPTY, CellState::EMPTY, CellState::EMPTY, CellState::EMPTY, CellState::EMPTY, CellState::EMPTY, CellState::EMPTY, CellState::EMPTY, CellState::EMPTY, CellState::EMPTY, CellState::EMPTY },
        { CellState::EMPTY, CellState::EMPTY, CellState::EMPTY, CellState::EMPTY, CellState::EMPTY, CellState::EMPTY, CellState::EMPTY, CellState::EMPTY, CellState::EMPTY, CellState::EMPTY, CellState::EMPTY, CellState::EMPTY },
        { CellState::EMPTY, CellState::EMPTY, CellState::EMPTY, CellState::EMPTY, CellState::EMPTY, CellState::EMPTY, CellState::EMPTY, CellState::EMPTY, CellState::EMPTY, CellState::EMPTY, CellState::EMPTY, CellState::EMPTY }
    };

    static constexpr float HUD_TOP = 32.0f;     // space above the HUD boxes
    static constexpr float HUD_MARGIN = 64.0f;  // space between the side boxes and the screen edge
    static constexpr int INITIAL_MIRRORS = 5;
    static constexpr LaserSide EMITTER_SIDE = LaserSide::LEFT;
    static constexpr int EMITTER_POS = 6;
    static constexpr const char* BEST_KEY = "BEST";

    LaserGrid* _grid = nullptr;

    bool _isSolved = false;

    int _best = 0;
    int _mirrorCount = 0;

    HudValue* _hudBest = nullptr;
    HudValue* _hudMirrors = nullptr;
    HudValue* _hudRemaining = nullptr;
    SolvedPanel* _panel = nullptr;

    void initGrid(const ax::Size& visibleSize, ax::Vec2 origin);
    void initPanel(const ax::Size& visibleSize, ax::Vec2 origin);
    void initBest(const ax::Size& visibleSize, ax::Vec2 origin);
    void initMirrors(const ax::Size& visibleSize, ax::Vec2 origin);
    void initRemaining(const ax::Size& visibleSize, ax::Vec2 origin);
    void initTouchListener();

    bool onTouchBegan(ax::Touch* touch, ax::Event* event);

    void setBest(int value);
    void setMirrors(int value);
    void setRemaining(int value) const;

    void showPanel();

    void loadBestValue();
    void saveBestValue() const;
};
