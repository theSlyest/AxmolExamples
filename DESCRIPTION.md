# Signal Lock

A small tuning game built with Axmol. A hidden station is somewhere on a radio dial, and a glitchy TV test pattern clears up as you get close to it. Tune onto the station, hold it, and lock it before the time runs out.

## Gameplay

- Hold **LEFT** or **RIGHT** (or **A** or **D**) to slide the tuning arrow along the dial. Press **R** to restart.
- The arrow moves at 0.3 of the dial per second. If you hold both directions, the key pressed last wins.
- The round starts with your first tuning key, and you have **45 seconds**.
- The picture on the screen is garbled when you are far from the station, and clears up inside 0.2 of the dial.
- Stay within 0.03 of the station for **0.6 seconds** and it locks: you score a point and a new station appears, at least 0.25 of the dial away.
- When the time runs out, the round is over and your score is shown.

There are no image assets: the dial and the TV are drawn with `DrawNode`, and the test pattern is generated pixel by pixel.

## What this example shows

| Concept | Where |
| --- | --- |
| Scene setup and wiring the parts together | `SignalLockScene` |
| Game rules with no engine dependency, reporting changes through callbacks | `SignalLockGame` |
| An injectable random source, so the rules can be tested deterministically | `SignalLockGame::Random` |
| A custom fragment shader with uniforms (jitter, RGB split, scanlines, noise) | `Shaders/tv_glitch.frag`, `SignalScreen` |
| Compiling an app shader for every backend and loading it by name | `CMakeLists.txt`, `SignalScreen::initShaderProgram` |
| A texture built from a pixel buffer | `SignalScreen::createPicture` |
| Clipping to a rounded rectangle with `ClippingNode` | `SignalScreen` |
| Procedural drawing with `DrawNode`, no image assets | `SignalDial`, `SignalScreen` |
| Keyboard input that tracks which keys are held | `SignalControls` |
| Views that only show state they are given | `SignalDial`, `SignalScreen`, `SignalHud`, `SignalGameOverLayer` |
| Unit tests with doctest | `Tests/` |

## Architecture

The scene only builds nodes and connects them. The rules know nothing about nodes, and the views know nothing about the rules.

```
SignalLockScene          builds the nodes, wires everything, forwards update(dt),
 │                       copies the game's state onto the views
 ├─ SignalLockGame       rules and state; calls onScoreChanged / onTimeChanged / onGameOver
 ├─ SignalControls       keyboard -> direction (-1, 0, +1) and onRestart
 ├─ SignalScreen         the TV and its glitch shader; setGlitchAmplitude(0..1)
 ├─ SignalDial           ruler and tuning arrow; setValue(0..1) and lockTo(0..1)
 ├─ SignalHud            score and time labels
 └─ SignalGameOverLayer  "ROUND OVER" overlay with the final score
```

Each frame the scene passes the held direction from `SignalControls` to `SignalLockGame::update`. The game moves the dial, measures the distance to the station and advances the lock and the clock. The scene then copies the dial value, the lock progress and the glitch amount onto the views, and updates the labels and the overlay from the game's callbacks.

### The glitch shader

`Shaders/tv_glitch.frag` is a fragment shader in Axmol's source format (`#version 310 es`, with `COLOR0`, `TEXCOORD0` and `SV_Target0` locations and a plain `fs_ub` uniform block). The build compiles it with `axslcc` into a program for each backend (MSL on Metal, GLSL on OpenGL, and so on), and `SignalScreen` loads the compiled program by name:

```cpp
ProgramManager::getInstance()->loadProgram(positionTextureColor_vert, "custom/tv_glitch_fs", VertexLayoutType::Sprite);
```

Each frame `SignalScreen` sets two uniforms, `u_time` and `u_amplitude`. The amplitude is the glitch amount from the game: at 0 the shader returns the plain picture, and at 1 it applies the full jitter, RGB split, scanlines and noise.

The shader is compiled from `CMakeLists.txt` rather than by putting it in `Source/shaders`. Axmol compiles that folder for every target, and the unit test target would then compile the same shader a second time, which CMake rejects.

### Try it: change the rules

The rules are constants at the top of `SignalLockGame`:

```cpp
static constexpr float TUNE_SPEED = 0.3f;   // dial widths per second
static constexpr float LOCK_DELAY = 0.6f;   // seconds on the station to lock it
static constexpr int START_TIME = 45;       // seconds per round
```

To make a round predictable while you experiment, give the game a fixed random source. The game calls it to choose each station:

```cpp
// Always put the station at 90% of the dial (or at the far end if that is too close).
_game = std::make_unique<SignalLockGame>([] { return 0.9f; });
```

## Tests

`SignalLockGame` has no nodes, so it runs in a plain test program. `Tests/SignalLockGameTest.cpp` uses [doctest](https://github.com/doctest/doctest), which Axmol bundles, and a fixed sequence of "random" numbers to cover:

- the start state, the clock waiting for the first tuning key, and a key held through a restart
- how stations are chosen: too close is rejected, exactly `LOCK_SAFE` is accepted, a stubborn random source falls back to the far end
- tuning speed, direction and the two ends of the dial
- the glitch amount: full when far, clear on the station, proportional in between
- locking: the score after `LOCK_DELAY`, the lock building up and resetting, a new station after each lock
- the clock: counting down, ending the round once, and starting a fresh round on reset

Enable the test target when configuring, build it, and run the executable:

```bash
axmol build -p osx -a arm64 -f -xc '-D_AX_TESTS=ON'
cmake --build build --target AxmolExamples_test --config RelWithDebInfo
build/bin/AxmolExamples_test/RelWithDebInfo/AxmolExamples_test.app/Contents/MacOS/AxmolExamples_test
```

Pass `-D_AX_TESTS=OFF` when you are done to go back to building only the game.

## Source layout

- `Source/SignalLockScene.*`: creates the nodes, connects them, and copies the state onto them.
- `Source/SignalLockGame.*`: the dial, the hidden station, locking, score and clock.
- `Source/SignalControls.*`: keyboard handling.
- `Source/SignalScreen.*`: the TV, its test-pattern texture and the shader program.
- `Source/SignalDial.*`: the ruler and the tuning arrow with its lock bar.
- `Source/SignalHud.*`, `Source/SignalGameOverLayer.*`: the on-screen labels and the end overlay.
- `Shaders/tv_glitch.frag`: the glitch fragment shader.
- `Tests/`: unit tests for `SignalLockGame`.
