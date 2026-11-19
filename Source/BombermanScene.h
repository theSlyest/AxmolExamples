#pragma once

#include "2d/Scene.h"
#include <memory>

#include "BomberControls.h"
#include "BomberEnemy.h"
#include "BomberGame.h"
#include "BomberGameOverLayer.h"
#include "BomberGrid.h"
#include "BomberHud.h"
#include "BomberPlayer.h"

class BombermanScene : public ax::Scene
{
public:
    BombermanScene() : ax::Scene() {};

    bool init() override;
    
    // implement the "static create()" method manually
    CREATE_FUNC(BombermanScene);

    void update(float dt) override;

    void reset();

private:
    const ax::Vec2 GRID_OFFSET = ax::Vec2(168.0f, 26.0f);

    BomberHud* _hud = nullptr;
    BomberGameOverLayer* _endLayer = nullptr;

    BomberGrid* _grid = nullptr;
    BomberPlayer* _player = nullptr;
    std::vector<BomberEnemy*> _enemies;

    BomberControls* _controls = nullptr;
    std::unique_ptr<BomberGame> _game;

    void initHud(const ax::Vec2& origin, const ax::Size& size);
    void initGrid(const ax::Vec2& origin);
    void initEndLayer();
    void initPlayer();
    void initEnemies();
    void initControls();
    void initGameCallbacks();

    // Copy the actors' positions from the game to their nodes.
    void syncActors() const;
};
