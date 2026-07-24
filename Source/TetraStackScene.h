#ifndef __TETRA_STACK_SCENE_H__
#define __TETRA_STACK_SCENE_H__

#include "axmol.h"
#include "TetraGrid.h"

class TetraStackScene : public ax::Scene
{
public:
    virtual bool init();
    
    // implement the "static create()" method manually
    CREATE_FUNC(TetraStackScene);

private:
    const float HUD_OFFSET = 300.0f;

    bool _isPlaying;
    int _score;
    int _linesCount;

    TetraGrid* _grid;

    ax::LayerColor* _resultLayer;
    ax::Label* _scoreLbl;
    ax::Label* _endScoreLbl;
    ax::Label* _linesLbl;
    ax::EventListenerKeyboard* _keyboardListener;

    ax::Size _nextBoxSize;
    std::vector<TetraCell*> _nextCells;
    std::queue<Tetromino> _nextMinos;

    void initScore(const ax::Vec2& origin);
    void initLines(const ax::Vec2& origin);
    void initGrid(const ax::Vec2& origin);
    void initNextBox(const ax::Vec2& origin);
    void initResultLayer(const ax::Size& size);
    void initKeyboardListener();

    void onKeyPressed(ax::EventKeyboard::KeyCode key);
    void onKeyReleased(ax::EventKeyboard::KeyCode key);
    void onBlockLocked(LinesData* data);

    void setScore(int score);
    void setLines(int lines);

    void reset();

    void pushNextBlock();
    void showNextBlock();

    void showResult();
};

#endif // __TETRA_STACK_SCENE_H__
