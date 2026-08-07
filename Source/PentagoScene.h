#pragma once

#include "2d/Scene.h"
#include "EventKeyboard.h"
#include "2d/Label.h"
#include "2d/Layer.h"
#include "Touch.h"
#include "PentagoBoard.h"

class PentagoScene : public ax::Scene
{
public:
    bool init() override;
    
    // implement the "static create()" method manually
    CREATE_FUNC(PentagoScene);

private:
    ax::Label* _turnLbl = nullptr;
    ax::Label* _endLbl = nullptr;
    ax::LayerColor* _endLayer = nullptr;
    PentagoBoard* _board      = nullptr;

    bool _isBlack = false;

    void initEndLayer(const ax::Vec2& origin, const ax::Vec2& size);

    void reset();

    void onKeyPressed(ax::EventKeyboard::KeyCode key);
    bool onTouch(ax::Touch* touch) const;
    void onBlockTurned();

    void updateTurnLabel() const;

    void showEndLayer(int result) const;
};
