# Grid Balancer

A small power-grid balancing game built with Axmol. A demand curve scrolls across a chart, and you keep the supply on top of it by raising and lowering three generators.

## Gameplay

- Click the **up** and **down arrows** under each generator. Each click moves the generator's target by a quarter of the range.
- The bar then glides toward its target at the generator's own rate: **FAST** (0.5 of the range per second), **MEDIUM** (0.25) and **SLOW** (0.125).
- The **supply** is the average of the three generators. The **demand** is a sine wave with a random amplitude and offset at every peak and trough, and its period shrinks from 12 s to 6 s as the game goes on.
- Keep the supply inside the **tolerance band** (±0.1 around the demand) to bank **uptime**.
- Every 4 seconds spent outside the band costs a **strike**. You have 3 strikes.
- When the strikes run out, the game ends. Your best uptime is saved between runs. Press **Play Again** to restart.

There are no image assets: everything is drawn with `DrawNode`.

## What this example shows

| Concept | Where |
| --- | --- |
| Scene setup and wiring the parts together | `BalancerScene` |
| Game rules that do not depend on the engine, reporting changes through callbacks | `BalancerGame` |
| Plain classes that can be compiled and tested without Axmol | `BalancerGame`, `DemandCurve`, `GeneratorModel` |
| Procedural signal generation with an injectable random source | `DemandCurve` |
| A bounded sample history, so a scrolling chart does not grow forever | `BalancerChart` |
| Procedural drawing with `DrawNode` | `BalancerChart`, `BalancerGenerator`, `BalancerValue` |
| Clipping a scrolling area with `ClippingNode` | `BalancerChart` |
| Buttons with `Menu` and `MenuItemLabel` | `BalancerGenerator`, `BalancerGameOverLayer` |
| Saving a value between runs with `UserDefault` | `BestScore` |
| Unit tests with doctest for the engine-free classes | `Tests/` |
| Views that only display state they are given | `BalancerHud`, `BalancerGameOverLayer`, `BalancerGenerator` |

## Architecture

The scene only builds nodes and connects them. The rules know nothing about nodes, and the views know nothing about the rules.

```
BalancerScene            creates the nodes, wires everything, forwards update(dt)
 ├─ BalancerGame         rules and state; calls onUpTimeChanged / onStrikesChanged /
 │   │                   onGraceChanged / onGameOver
 │   ├─ DemandCurve      the demand the player has to follow
 │   └─ GeneratorModel×3 value, target and rate of each generator
 ├─ BalancerGenerator×3  bar and arrow buttons; fires onIncrease / onDecrease
 ├─ BalancerChart        draws demand, supply and the tolerance band from a bounded history
 ├─ BalancerHud          uptime, strikes and best boxes
 ├─ BalancerGameOverLayer panel with the result and a Play Again button
 └─ BestScore            best uptime saved with UserDefault
```

Each frame the scene calls `BalancerGame::update`, then copies the generator values and the supply and demand into the views. When the player clicks an arrow, the generator view fires a callback and the scene forwards it to the game.

### The chart keeps a bounded history

`BalancerChart` samples demand and supply 30 times per second into a fixed-size buffer and redraws the curves from it. The newest sample stays at 96% of the chart width, and older ones scroll left until they are dropped. The geometry therefore stays constant however long the game runs.

### Try it: change the rules

`BalancerGame` takes a `Config`, so most rules are one-line changes in `BalancerScene::initGame`:

```cpp
BalancerGame::Config config;
config.generatorRates = { 0.5f, 0.25f, 0.125f };
config.tolerance = 0.15f;     // easier: a wider band
config.gracePeriod = 2.0f;    // harder: strikes come faster
config.strikes = 5;
```

To make the demand predictable while you experiment, give the game a fixed random source. `DemandCurve` calls it to choose each amplitude:

```cpp
const auto middle = [](float min, float max) { return (min + max) / 2.0f; };
_game = std::make_unique<BalancerGame>(config, middle);
```

## Tests

The classes that do not depend on Axmol (`GeneratorModel`, `DemandCurve` and `BalancerGame`) have unit tests in `Tests/`, written with [doctest](https://github.com/doctest/doctest), which Axmol bundles. The demand curve and the game take an injectable random source, so the tests are deterministic.

Enable the test target when configuring, build it, and run the executable:

```bash
axmol build -p osx -a arm64 -f -xc '-D_AX_TESTS=ON'
cmake --build build --target AxmolExamples_test --config RelWithDebInfo
build/bin/AxmolExamples_test/RelWithDebInfo/AxmolExamples_test.app/Contents/MacOS/AxmolExamples_test
```

Pass `-D_AX_TESTS=OFF` when you are done to go back to building only the game.

## Source layout

- `Source/BalancerScene.*`: creates the nodes and connects them.
- `Source/BalancerGame.*`: strikes, grace time, uptime and game over; owns the generators and the demand.
- `Source/DemandCurve.*`: the demand signal, with an injectable random source.
- `Source/GeneratorModel.*`: one generator's value, target and rate.
- `Source/BalancerGenerator.*`: the generator bar and its arrow buttons.
- `Source/BalancerChart.*`: the scrolling chart with its legend and grace marker.
- `Source/BalancerHud.*`, `Source/BalancerValue.*`: the top boxes and the single labeled box they use.
- `Source/BalancerGameOverLayer.*`: the end-of-game panel.
- `Source/BestScore.*`: the saved best uptime.
- `Tests/`: unit tests for the engine-free classes.
