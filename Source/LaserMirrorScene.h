#ifndef __LASER_MIRROR_SCENE_H__
#define __LASER_MIRROR_SCENE_H__

#include "axmol.h"
#include "HudValue.h"
#include "LaserGrid.h"
#include "SolvedPanel.h"

class LaserMirrorScene : public ax::Scene
{
public:
    static ax::Scene* createScene();

    virtual bool init();
    
    // implement the "static create()" method manually
    CREATE_FUNC(LaserMirrorScene);

    virtual void onExit() override;

    void reset();

private:
    const std::vector<std::vector<CellState>> INITIAL_GRID {
        { CellState::EMPTY, CellState::EMPTY, CellState::EMPTY, CellState::EMPTY, CellState::EMPTY, CellState::EMPTY, CellState::EMPTY, CellState::EMPTY, CellState::EMPTY, CellState::EMPTY, CellState::EMPTY, CellState::EMPTY },
        { CellState::EMPTY, CellState::EMPTY, CellState::EMPTY, CellState::EMPTY, CellState::WALL, CellState::EMPTY, CellState::EMPTY, CellState::EMPTY, CellState::EMPTY, CellState::EMPTY, CellState::EMPTY, CellState::EMPTY },
        { CellState::EMPTY, CellState::EMPTY, CellState::EMPTY, CellState::WALL, CellState::EMPTY, CellState::WALL, CellState::EMPTY, CellState::EMPTY, CellState::EMPTY, CellState::EMPTY, CellState::EMPTY, CellState::EMPTY },
        { CellState::EMPTY, CellState::EMPTY, CellState::WALL, CellState::EMPTY, CellState::EMPTY, CellState::TARGET, CellState::WALL, CellState::EMPTY, CellState::EMPTY, CellState::EMPTY, CellState::EMPTY, CellState::EMPTY },
        { CellState::EMPTY, CellState::EMPTY, CellState::EMPTY, CellState::EMPTY, CellState::EMPTY, CellState::WALL, CellState::EMPTY, CellState::EMPTY, CellState::EMPTY, CellState::EMPTY, CellState::EMPTY, CellState::EMPTY },
        { CellState::EMPTY, CellState::EMPTY, CellState::EMPTY, CellState::EMPTY, CellState::EMPTY, CellState::EMPTY, CellState::EMPTY, CellState::EMPTY, CellState::EMPTY, CellState::EMPTY, CellState::EMPTY, CellState::EMPTY },
        { CellState::EMPTY, CellState::EMPTY, CellState::EMPTY, CellState::EMPTY, CellState::EMPTY, CellState::EMPTY, CellState::EMPTY, CellState::EMPTY, CellState::EMPTY, CellState::EMPTY, CellState::EMPTY, CellState::EMPTY },
        { CellState::EMPTY, CellState::EMPTY, CellState::EMPTY, CellState::EMPTY, CellState::EMPTY, CellState::EMPTY, CellState::EMPTY, CellState::EMPTY, CellState::EMPTY, CellState::EMPTY, CellState::EMPTY, CellState::EMPTY }
    };

    const float HUD_OFFSET = 32.0f;
    const int INITIAL_MIRRORS = 5;
    const LaserSide EMITTER_SIDE = LaserSide::LEFT;
    const int EMITTER_POS = 6;
    const char* BEST_KEY = "BEST";

    LaserGrid* _grid;

    ax::EventListenerTouchOneByOne* _touchListener;

    bool _isSolved;

    int _best;
    int _mirrorCount;

    HudValue* _hudBest;
    HudValue* _hudMirrors;
    HudValue* _hudRemaining;
    SolvedPanel* _panel;

    void initGrid(const ax::Size& visibleSize, const ax::Vec2 origin);
    void initPanel(const ax::Size& visibleSize, const ax::Vec2 origin);
    void initBest(const ax::Size& visibleSize, const ax::Vec2 origin);
    void initMirrors(const ax::Size& visibleSize, const ax::Vec2 origin);
    void initRemaining(const ax::Size& visibleSize, const ax::Vec2 origin);
    void initTouchListener();

    bool onTouchBegan(ax::Touch* touch, ax::Event* event);

    void setBest(int value);
    void setMirrors(int value);
    void setRemaining(int value);

    void showPanel();

    void loadBestValue();
    void saveBestValue();
};

#endif // __LASER_MIRROR_SCENE_H__
