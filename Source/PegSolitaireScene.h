#pragma once

#include "axmol.h"
#include "PegBoard.h"

class PegSolitaireScene : public ax::Scene
{
public:
    static ax::Scene* createScene();

    bool init() override;
    
    // implement the "static create()" method manually
    CREATE_FUNC(PegSolitaireScene);

private:
    const float UI_OFFSET_X = 24.0f;
    const float UI_OFFSET_Y = 48.0f;

    int _pegs = 0;

    ax::Label* _pegsLbl = nullptr;
    ax::Label* _endLbl = nullptr;
    ax::LayerColor* _endPanel = nullptr;
    PegBoard* _board = nullptr;
    PegCoords _selected;

    ax::EventListenerKeyboard* _keyboardListener = nullptr;
    ax::EventListenerTouchOneByOne* _touchListener = nullptr;

    void initPegs(const ax::Vec2& origin, const ax::Size& size);
    void initBoard(const ax::Vec2& origin, const ax::Size& size);
    void initEndPanel(const ax::Vec2& origin, const ax::Size& size);
    void initKeyboardListener();
    void initTouchListener();

    void setPegs(int pegs);
    void endGame(bool solved);

    void reset();

    void onKeyPressed(ax::EventKeyboard::KeyCode key);
    bool onTouch(ax::Touch* touch);

    bool isValidMove(const PegCoords& coords) const;
};
