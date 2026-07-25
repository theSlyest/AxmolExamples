#ifndef __MINES_SCENE_H__
#define __MINES_SCENE_H__

#include "axmol.h"
#include "MineGrid.h"

enum class MineState { EMPTY, ACTIVE, FINISHED };

class MineScene : public ax::Scene
{
public:
    static ax::Scene* createScene();

    virtual bool init();
    
    // implement the "static create()" method manually
    CREATE_FUNC(MineScene);

private:
    const int MINES = 10;
    const std::string TIMER_KEY = "TimerUpdate";

    int _minesLeft;
    int _timer;

    MineState _state;
    MineGrid* _grid;

    ax::EventListenerMouse* _mouseListener;
    ax::Label* _minesLbl;
    ax::Label* _timerLbl;
    ax::Label* _endTimerLbl;
    ax::Label* _resultLbl;
    ax::LayerColor* _resultLayer;

    void initGrid(const ax::Size& size, const ax::Vec2& origin);
    void initMinesLabel(const ax::Size& size, const ax::Vec2& origin);
    void initTimerLabel(const ax::Size& size, const ax::Vec2& origin);
    void initResetButton(const ax::Size& size, const ax::Vec2& origin);
    void initResultLayer(const ax::Size& size, const ax::Vec2& origin);
    void initMouseListener();

    bool onMouseDown(ax::EventMouse *event);
    bool onMouseMove(ax::EventMouse *event);

    void setMinesLeft(int value);

    void startTimer();
    void stopTimer();

    void showResult(bool victory);

    void reset();
};

#endif // __MINES_SCENE_H__
