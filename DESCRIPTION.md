# Bomberman

A minimal Bomberman built with Axmol. It covers basic gameplay only: no power-ups, no level progression.

## Gameplay

- Move with the **arrow keys**, drop a bomb with **Space**, restart with **R**.
- You have **3 lives** and **120 seconds**.
- A bomb explodes after 2 seconds. The blast reaches 2 tiles in each direction, destroys walls, stops at steel, and sets off other bombs it touches.
- You can have one bomb on the grid at a time.
- Touching an enemy or standing in fire costs a life. You are briefly invincible after respawning.
- Each enemy killed by a blast gives 100 points. Kill all three to win.
- The game ends when you win, lose all lives, or the timer reaches 0.

## What this example shows

| Concept | Where |
| --- | --- |
| Scene setup and wiring the parts together | `BombermanScene` |
| Frame loop with `update(dt)` | `BombermanScene::update`, `BomberGame::update` |
| Keyboard input (`EventListenerKeyboard`) | `BomberControls` |
| Game rules and state with no engine dependency, reporting changes through callbacks | `BomberGame`, `GridModel`, `ActorModel` |
| Views that only draw state they are given | `BomberGrid`, `GridTile`, `BomberBomb`, `BomberPlayer`, `BomberEnemy` |
| HUD labels and a game-over overlay as their own nodes | `BomberHud`, `BomberGameOverLayer` |
| Grid-locked movement (move tile to tile, turn only at tile centres) | `BomberGame::updatePlayer`, `BomberGame::updateEnemy` |
| Swappable enemy AI (strategy pattern) with an injectable random source | `EnemyBrain`, `RandomWalkBrain` |
| Tile state machine (`SAFE`, `WALL`, `STEEL`, `BOMB`) and timers | `GridModel` |
| Chain-reaction explosions, with the blast range as a constant | `GridModel::detonate`, `GridModel::BLAST_RANGE` |
| Grid coordinates vs. pixel positions kept in one place | `GridModel::coordsToPosition`, `positionToCoords`, `neighborPosition`, `centerCoords` |
| Redrawing only what changed (model notifies, view refreshes) | `GridModel::onTileChanged`, `BomberGrid::refresh` |
| Procedural drawing with `DrawNode`, no image assets | `GridTile`, `BomberPlayer`, `BomberEnemy`, `BomberBomb` |
| Unit tests with doctest for the rules, the grid and the AI | `Tests/` |

## Architecture

The game state and the rules are plain C++ classes with no nodes and no drawing. The scene only builds nodes, connects them, and copies the state onto the screen.

```
BombermanScene           builds the nodes, wires everything, forwards update(dt),
 │                       copies the actors' positions onto their nodes
 ├─ BomberGame           rules; owns the state below; calls onScoreChanged /
 │   │                   onLivesChanged / onTimeChanged / onGameOver
 │   ├─ GridModel        tiles, bombs, fire and explosions; calls onTileChanged
 │   ├─ ActorModel       position, direction and destination of the player and enemies
 │   └─ EnemyBrain       decides where an enemy goes next
 ├─ BomberControls       keyboard -> held direction + onDirectionPressed / onBomb / onRestart
 ├─ BomberGrid           draws the GridModel: a GridTile per cell and a BomberBomb per bomb
 ├─ BomberPlayer, BomberEnemy   how the actors look
 ├─ BomberHud            score, time and lives labels
 └─ BomberGameOverLayer  overlay shown when the game ends
```

Each frame the scene passes the held direction from `BomberControls` to `BomberGame::update`. The game moves the actors and updates the `GridModel`, which reports every tile that changed; `BomberGrid` redraws just those tiles. The scene then copies the actors' positions to their nodes, and updates the HUD or shows the overlay from the game's callbacks. The commit history follows the refactor step by step, so it also works as a walkthrough.

### The grid model owns the geometry and the explosions

`GridModel` is the only place that knows the tile size, the blast range and how fire spreads. Game code and enemy brains ask it questions instead of doing the math:

```cpp
GridModel::neighborPosition(pos, direction);  // position of the next tile
grid.isSafe(pos);                             // can something walk here?
GridModel::centerCoords(actor.getPosition()); // tile under an actor
grid.detonate(col, row);                      // spread fire, break walls, chain bombs
```

Changing `GridModel::TILE_SIZE` or `GridModel::BLAST_RANGE` changes the game without touching the rules.

### Try it: change how enemies behave

Enemies pick their next direction through `EnemyBrain`. To change their behavior, implement it. A brain that chases the player needs to know where the player is, which lives in the game, so give it a way to ask:

```cpp
class ChaseBrain : public EnemyBrain
{
public:
    std::function<ax::Vec2()> target; // where to chase

    ax::Vec2 chooseDirection(const GridModel& grid, const ax::Vec2& pos) override
    {
        // Pick the free direction that gets closest to target().
    }
};
```

Then give it to `BomberGame` when the scene creates it:

```cpp
auto brain = std::make_unique<ChaseBrain>();
const auto chase = brain.get();
_game = std::make_unique<BomberGame>(std::move(brain));
chase->target = [this] { return _game->getPlayer().getPosition(); };
```

## Tests

The game state and rules have no nodes, so they run in a plain test program. `Tests/` holds unit tests written with [doctest](https://github.com/doctest/doctest), which Axmol bundles:

- `GridModelTest`: the layout, tile geometry, explosions (range, steel, walls, chain reactions), bombs, fire burning out, reset and change notifications
- `ActorModelTest`: reaching a destination in each direction and spawn invincibility
- `BomberGameTest`: movement, bombs, burning, enemies killed and the win, contact with enemies, losing lives, the timer, game over and reset
- `RandomWalkBrainTest`: it only picks free tiles, avoids bombs and stays still when boxed in
- `DirectionsTest`: the direction constants

The tests make the game deterministic with a brain that never moves, or a random source that always picks the first option. The views (`BomberGrid`, `GridTile` and the others) need the renderer, so they are covered by playing the game.

Enable the test target when configuring, build it, and run the executable:

```bash
axmol build -p osx -a arm64 -f -xc '-D_AX_TESTS=ON'
cmake --build build --target AxmolExamples_test --config RelWithDebInfo
build/bin/AxmolExamples_test/RelWithDebInfo/AxmolExamples_test.app/Contents/MacOS/AxmolExamples_test
```

Pass `-D_AX_TESTS=OFF` when you are done to go back to building only the game.

## Source layout

- `Source/BombermanScene.*`: creates the nodes, connects them, and copies the state onto them.
- `Source/BomberGame.*`: score, lives, time, movement, bombs, burning, win and lose; owns the grid and the actors' state.
- `Source/GridModel.*`: the map, bombs, fire, explosions and the coordinate helpers.
- `Source/ActorModel.*`: the movement state shared by the player and the enemies.
- `Source/EnemyBrain.h`, `Source/RandomWalkBrain.*`: the enemy decision interface and its default.
- `Source/BomberControls.*`: keyboard handling. `Directions.h` holds the four grid directions.
- `Source/BomberGrid.*`, `Source/GridTile.*`, `Source/BomberBomb.*`: draw the grid, its tiles and its bombs.
- `Source/BomberPlayer.*`, `Source/BomberEnemy.*`: the blue player and the green enemies.
- `Source/BomberHud.*`, `Source/BomberGameOverLayer.*`: the on-screen labels and the end overlay.
- `Tests/`: unit tests for the game state, the rules and the enemy AI.
