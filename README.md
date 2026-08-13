# Axmol Examples

A collection of small games, gameplay mechanics, and technical demonstrations built with [Axmol Engine](https://axmol.dev/) and C++.

The purpose of this repository is to provide compact, practical examples that can be studied, modified, and reused when learning Axmol or prototyping game mechanics.

## Repository Structure

Each example is stored in its own Git branch.

The `master` branch contains the base Axmol project used as a starting point. To explore an example, switch to the corresponding branch:

```bash
git clone https://github.com/theSlyest/AxmolExamples.git
cd AxmolExamples
git switch <example-branch>
```

For example:

```bash
git switch Minesweeper
```

You can list all available example branches with:

```bash
git branch --remotes
```

## Available Examples

### Mini-games and puzzles

| Branch         | Description                                                             |
| -------------- | ----------------------------------------------------------------------- |
| `Bomberman`    | A small game inspired by classic grid-based bomb mechanics.             |
| `BrickBreaker` | A brick-breaking game with paddle, ball, and collision mechanics.       |
| `Cooking`      | A cooking-themed gameplay prototype.                                    |
| `CrossTheRoad` | A road-crossing game based on movement, timing, and obstacle avoidance. |
| `DiskTower`    | A Tower of Hanoi-style puzzle.                                          |
| `GridBalancer` | A grid-based balancing puzzle.                                          |
| `Minesweeper`  | An implementation of the classic Minesweeper game.                      |
| `Nonogram`     | A picture-logic puzzle based on row and column clues.                   |
| `PegSolitaire` | An implementation of the classic peg solitaire puzzle.                  |
| `Pentago`      | A board game featuring piece placement and rotating board sections.     |
| `PoseMatch`    | A pose-matching gameplay prototype.                                     |
| `Reversi`      | An implementation of the classic Reversi/Othello board game.            |
| `RushHour`     | A sliding-block puzzle inspired by Rush Hour.                           |
| `ShadowShape`  | A shape and shadow-matching game.                                       |
| `Snake`        | An implementation of the classic Snake game.                            |
| `Sudoku`       | An implementation of the classic number-placement puzzle.               |
| `TetraStack`   | A block-stacking game using tetromino-like pieces.                      |
| `WordGame`     | A word-based game prototype.                                            |

### Action and gameplay mechanics

| Branch             | Description                                                    |
| ------------------ | -------------------------------------------------------------- |
| `Escorting`        | An escort-oriented gameplay prototype.                         |
| `LaserMirror`      | A laser-routing mechanic using reflective objects.             |
| `RippleTank`       | A tank-based gameplay experiment featuring ripple effects.     |
| `SideShooter`      | A side-view shooting game prototype.                           |
| `SignalLock`       | A signal-based locking or synchronization mechanic.            |
| `StealthDetection` | A demonstration of visibility and stealth-detection mechanics. |
| `Teleporter`       | A teleportation gameplay mechanic.                             |

### Technical and narrative examples

| Branch        | Description                                                           |
| ------------- | --------------------------------------------------------------------- |
| `Clones`      | A gameplay experiment involving duplicated or synchronized entities.  |
| `PathFinding` | A demonstration of pathfinding and grid navigation.                   |
| `VisualNovel` | A basic visual-novel structure with narrative and dialogue mechanics. |

> The examples are intentionally small and focused. They are designed as learning resources and prototypes rather than production-ready games.

## Requirements

* A C++ development environment
* [CMake](https://cmake.org/) 3.22 or later
* [Axmol Engine](https://github.com/axmolengine/axmol)
* The platform-specific tools required by Axmol

The base project currently targets Axmol Engine **2.11.4**.

Refer to the [Axmol documentation](https://axmol.dev/manual/latest/) for engine installation and platform setup instructions.

## Building an Example

First, install and configure Axmol Engine. Then clone this repository and select the example you want to build:

```bash
git clone https://github.com/theSlyest/AxmolExamples.git
cd AxmolExamples
git switch <example-branch>
```

Build the project using the standard Axmol workflow for your target platform.

For example, after configuring the Axmol command-line tools:

```bash
axmol build
```

Available targets and build options may depend on your operating system and Axmol installation. Consult the Axmol build documentation for the appropriate platform-specific command.

## Learning from the Examples

These projects may be useful for exploring topics such as:

* Scene and node management
* Touch, mouse, and keyboard input
* Grid-based gameplay
* Collision detection
* Movement and animation
* Board-game rules
* Pathfinding
* State management
* Puzzle validation
* User-interface implementation
* Reusable gameplay components

Because each example lives in an independent branch, its implementation can evolve without affecting the others.

## Contributions

Contributions, improvements, and bug fixes are welcome.

If you would like to contribute:

1. Fork the repository.
2. Create a branch from the example you want to improve.
3. Make and test your changes.
4. Open a pull request describing the changes.

For a new example, please keep the implementation compact and focused on a clearly identifiable game or technical concept.

## Disclaimer

This repository is primarily intended for experimentation and education. The examples may simplify architecture, error handling, content pipelines, or platform-specific considerations in order to keep the demonstrated concepts easy to understand.

## License

This repository is licensed under the MIT License.

You are free to use, modify, and distribute the source code, including in
commercial projects, provided that the original copyright notice and license
are retained. See the [LICENSE](LICENSE) file for details.

Third-party assets and dependencies may be subject to their own licenses.