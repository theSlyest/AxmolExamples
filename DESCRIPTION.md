# Ripple Tank

A small ripple game built with Axmol. A water surface is a mesh that ripples deform, and four contacts drift across it. Click the water to send a ripple, and score every contact it reaches.

## Gameplay

- Click the surface to send a ripple. The first click on the water starts the round; clicks elsewhere do nothing.
- Four orange **contacts** drift slowly across the water and bounce off its edges.
- Every contact within 150 pixels of your click scores a point and reappears somewhere else, away from the ripple.
- The round lasts **45 seconds** from your first click. Press **R** to restart at any time.
- Ripples are real waves: they spread out, fade and add up when they overlap, so waves from several clicks interfere.

There are no image assets: the water's grid is drawn into a texture, and the contacts are drawn with `DrawNode`.

## What this example shows

| Concept | Where |
| --- | --- |
| Scene setup and wiring the parts together | `RippleTankScene` |
| A mesh deformed every frame (`PolygonInfo` and `V3F_C4B_T2F` vertices on a `Sprite`) | `Water` |
| A texture built from a pixel buffer | `Water::createSurface` |
| Waves as a height function: a cosine around the wave front with a Gaussian envelope, added up over the ripples | `RippleField::heightAt` |
| Wave physics with no engine dependency | `RippleField` |
| Game rules with no engine dependency, reporting changes through callbacks | `TankGame` |
| An injectable random source, so the rules can be tested deterministically | `TankGame::Random` |
| Skipping work while the water is at rest | `Water::update` |
| Touch and keyboard input turned into commands | `RippleControls` |
| Views that only show the state they are given | `Water`, `Contact`, `RippleHud`, `StartPrompt`, `RoundOverLayer` |
| Unit tests with doctest | `Tests/` |

## Architecture

The scene only builds nodes and connects them. The rules and the wave physics know nothing about nodes, and the views know nothing about the rules.

```
RippleTankScene        builds the nodes, wires everything, forwards update(dt),
 │                     moves the contact nodes to where the game has them
 ├─ TankGame           rules; calls onScoreChanged / onTimerChanged / onStateChanged
 ├─ RippleControls     touches -> onTouch, R -> onRestart
 ├─ Water              the mesh and its grid texture; draws the ripples of
 │   │                 a RippleField, and locate(touch) / sendRipple(position)
 │   ├─ RippleField    the ripples and the height of the surface at any point
 │   └─ Contact ×4     a drawn disc each (children of the water)
 ├─ RippleHud          score and timer labels
 ├─ StartPrompt        "Click the surface to send a ripple"
 └─ RoundOverLayer     the final score and "Press R to restart"
```

When the player touches the screen, `RippleControls` hands the touch to the scene. The scene asks `Water::locate` where it landed. Touches outside the water are ignored. On the water, the scene tells `TankGame` to start the round (the first touch only), sends the ripple with `Water::sendRipple` for the picture, and calls `TankGame::strike` for the score. Every frame, the scene lets the game advance and moves the contact nodes to its positions; the game's callbacks update the HUD and show or hide the layers.

### The wave

`RippleField` keeps the ripples and answers one question: how high is the surface at this point? A ripple spreads at 150 pixels per second and fades over one second. For every mesh vertex, each ripple that is close to it adds

```
cos((distance - radius) * 0.1) * amplitude * exp(-(|distance - radius| / 40)²)
```

where `distance` is the vertex's distance from the ripple's origin and `radius` is how far the ripple has spread. Only vertices within 120 pixels of the wave front move at all. `Water` calls this for each of its 35×35 vertices and sets the vertex's height, and skips the whole update when no ripple is on the water.

The score does not wait for the wave to arrive: a click scores every contact within `RippleField::REACH`, which is the distance a ripple travels in its lifespan.

### Try it: change the game

The rules and the wave are constants and are easy to change:

```cpp
// TankGame.h
static constexpr int START_TIME = 45;          // seconds per round
static constexpr int CONTACT_COUNT = 4;
static constexpr float CONTACT_SPEED = 16.0f;  // pixels per second

// RippleField.h
static constexpr float SPEED = 150.0f;         // how fast a ripple spreads
static constexpr float LIFESPAN = 1.0f;        // and how long it lasts
```

A ripple's reach is its speed times its lifespan, so making ripples spread faster or last longer also makes them score from further away. For a predictable game while you experiment, give `TankGame` a fixed random source. It is called for every position and direction:

```cpp
// Every contact in the middle of the water, drifting to the right.
_game = std::make_unique<TankGame>(size, [](float min, float max) { return (min + max) / 2.0f; });
```

## Tests

`RippleField` and `TankGame` have no nodes, so they run in a plain test program. `Tests/` uses [doctest](https://github.com/doctest/doctest), which Axmol bundles, and a scripted random source:

- `RippleFieldTest`: ripples spreading, fading and expiring, aging separately, and the height of the surface (flat when calm, as high as the amplitude on the front, the same in every direction, dying away from the front, zero outside the window, and overlapping ripples adding up)
- `TankGameTest`: the contacts scattered and drifting, nothing moving until the first touch, bouncing off all four edges, ripples scoring every contact reached (including one exactly at the reach) and sending it elsewhere, never respawning inside the reach, the timer, the round ending once, and reset

Enable the test target when configuring, build it, and run the executable:

```bash
axmol build -p osx -a arm64 -f -xc '-D_AX_TESTS=ON'
cmake --build build --target AxmolExamples_test --config RelWithDebInfo
build/bin/AxmolExamples_test/RelWithDebInfo/AxmolExamples_test.app/Contents/MacOS/AxmolExamples_test
```

Pass `-D_AX_TESTS=OFF` when you are done to go back to building only the game.

## Source layout

- `Source/RippleTankScene.*`: creates the nodes, connects them, and moves the contacts to where the game has them.
- `Source/TankGame.*`: the contacts, the scoring, the timer and the three states of a round.
- `Source/RippleField.*`: the ripples and the height of the surface.
- `Source/Water.*`: the deformable mesh, its grid texture, and the ripple pictures.
- `Source/Contact.*`: how a contact looks.
- `Source/RippleControls.*`: touch and keyboard handling.
- `Source/RippleHud.*`, `Source/StartPrompt.*`, `Source/RoundOverLayer.*`, `Source/Theme.h`: the labels, the two messages and the colors they share.
- `Tests/`: unit tests for `RippleField` and `TankGame`.
