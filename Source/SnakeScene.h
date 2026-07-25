#pragma once

#include "axmol.h"
#include "Grid.h"

class SnakeScene : public ax::Scene
{
public:
    bool init() override;

    // implement the "static create()" method manually
    CREATE_FUNC(SnakeScene);

private:
    Grid* _grid = nullptr;
    ax::Label* _scoreLabel = nullptr;
    ax::EventListenerKeyboard* _keyboardListener = nullptr;
    ax::Layer* _gameOverLayer = nullptr;
    bool _isGameOver = false;

    void startGame();
    void restartGame();
    bool isGameOver() const { return _gameOverLayer->isVisible(); }
};
