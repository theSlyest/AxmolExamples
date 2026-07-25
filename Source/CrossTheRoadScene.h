#pragma once

#include "BaseBox.h"
#include "axmol.h"
#include "Obstacle.h"

class CrossTheRoadScene : public ax::Scene
{
public:
    bool init() override;
    
    // implement the "static create()" method manually
    CREATE_FUNC(CrossTheRoadScene);

private:
    ax::Label* _scoreLabel = nullptr;
    ax::EventListenerKeyboard* _keyboardListener = nullptr;

    BaseBox* _safeZone = nullptr;
    BaseBox* _player = nullptr;
    std::vector<std::list<Obstacle*>> _lanes;

    int _score = 0;
    int _discreteWidth = 0;
    int _discreteHeight = 0;
    int _lanesCount = 14;

    void resetGame();
    void spawnObstacle();

    void onKeyPressed(ax::EventKeyboard::KeyCode key);
    void onObstacleMove(const ax::EventCustom* event);
    void onObstacleOut(const ax::EventCustom* event);

    bool checkIntersection(const Obstacle* obstacle) const;
    void laneChanged();

    void updateScoreLabel() const;
};
