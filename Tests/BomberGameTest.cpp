#include "doctest/doctest.h"

#include <memory>

#include "BomberGame.h"
#include "Directions.h"
#include "RandomWalkBrain.h"

namespace {
constexpr float FRAME = 1.0f / 60.0f;

// Enemies that never move.
class StillBrain : public EnemyBrain
{
public:
    ax::Vec2 chooseDirection(const GridModel&, const ax::Vec2&) override { return ax::Vec2::ZERO; }
};

std::unique_ptr<BomberGame> stillGame()
{
    return std::make_unique<BomberGame>(std::make_unique<StillBrain>());
}

ax::Vec2 at(const int col, const int row)
{
    return GridModel::coordsToPosition({ col, row });
}

// Run the game for `seconds` at 60 frames per second with `key` held.
void run(BomberGame& game, const float seconds, const ax::Vec2& key = ax::Vec2::ZERO)
{
    for (float t = 0.0f; t < seconds; t += FRAME)
        game.update(FRAME, key);
}

// Move the player somewhere nothing can hurt it, so a blast can be watched safely.
void stepAway(BomberGame& game)
{
    game.getPlayer().setPosition(at(5, 9));
}
}

TEST_SUITE("BomberGame start") {
    TEST_CASE("reset starts a game with full lives, full time and no score") {
        auto game = stillGame();
        int lives = -1, time = -1, score = -1;
        game->onLivesChanged = [&](const int v) { lives = v; };
        game->onTimeChanged = [&](const int v) { time = v; };
        game->onScoreChanged = [&](const int v) { score = v; };

        game->reset();

        CHECK_FALSE(game->isOver());
        CHECK(game->getLives() == BomberGame::START_LIVES);
        CHECK(game->getTime() == BomberGame::START_TIME);
        CHECK(game->getScore() == 0);
        CHECK(lives == BomberGame::START_LIVES);
        CHECK(time == BomberGame::START_TIME);
        CHECK(score == 0);
    }

    TEST_CASE("the player starts in a corner and the enemies in the others") {
        auto game = stillGame();
        game->reset();

        CHECK(game->getPlayer().getPosition() == at(1, 1));
        REQUIRE(game->getEnemies().size() == BomberGame::ENEMY_COUNT);
        CHECK(game->getEnemies()[0].actor.getPosition() == at(1, 9));
        CHECK(game->getEnemies()[1].actor.getPosition() == at(11, 1));
        CHECK(game->getEnemies()[2].actor.getPosition() == at(11, 9));
        for (const auto& enemy : game->getEnemies())
            CHECK(enemy.alive);
    }

    TEST_CASE("a game that was never started is over and ignores updates") {
        auto game = stillGame();

        CHECK(game->isOver());
        game->update(5.0f, ax::Vec2::ZERO);
        CHECK(game->getTime() == 0);
    }
}

TEST_SUITE("BomberGame player movement") {
    TEST_CASE("the player can step onto a free tile but not into steel") {
        auto game = stillGame();
        game->reset();

        game->movePlayer(DIR_LEFT); // steel
        CHECK(game->getPlayer().getDirection() == ax::Vec2::ZERO);

        game->movePlayer(DIR_RIGHT);
        CHECK(game->getPlayer().getDirection() == DIR_RIGHT);
        CHECK(game->getPlayer().getDestination() == at(2, 1));
    }

    TEST_CASE("the player walks to the next tile and stops at a wall") {
        auto game = stillGame();
        game->reset();

        game->movePlayer(DIR_RIGHT);
        run(*game, 0.6f, DIR_RIGHT); // the tile after (2, 1) is a wall

        CHECK(game->getPlayer().getPosition().x == doctest::Approx(at(2, 1).x));
        CHECK(game->getPlayer().getPosition().y == doctest::Approx(at(2, 1).y));
        CHECK(game->getPlayer().getDirection() == ax::Vec2::ZERO);
    }

    TEST_CASE("the player keeps walking while the key is held and the way is free") {
        auto game = stillGame();
        game->reset();
        for (int col = 3; col <= 6; ++col)
            game->getGrid().setState(col, 1, TileState::SAFE);

        game->movePlayer(DIR_RIGHT);
        run(*game, 0.5f, DIR_RIGHT);

        CHECK(game->getPlayer().getPosition().x > at(3, 1).x - 1.0f);
        CHECK(game->getPlayer().getDirection() == DIR_RIGHT);
    }

    TEST_CASE("releasing the key stops the player on the next tile center") {
        auto game = stillGame();
        game->reset();
        for (int col = 3; col <= 6; ++col)
            game->getGrid().setState(col, 1, TileState::SAFE);

        game->movePlayer(DIR_RIGHT);
        run(*game, 0.5f, ax::Vec2::ZERO);

        CHECK(game->getPlayer().getPosition().x == doctest::Approx(at(2, 1).x));
        CHECK(game->getPlayer().getDirection() == ax::Vec2::ZERO);
    }

    TEST_CASE("the player can turn back before reaching the next tile") {
        auto game = stillGame();
        game->reset();

        game->movePlayer(DIR_RIGHT);
        run(*game, 0.05f, DIR_RIGHT);
        game->movePlayer(DIR_LEFT);

        CHECK(game->getPlayer().getDirection() == DIR_LEFT);
        CHECK(game->getPlayer().getDestination() == at(1, 1));

        run(*game, 0.4f, DIR_LEFT);
        CHECK(game->getPlayer().getPosition().x == doctest::Approx(at(1, 1).x));
    }
}

TEST_SUITE("BomberGame bombs") {
    TEST_CASE("dropping a bomb plants it on the player's tile, one at a time") {
        auto game = stillGame();
        game->reset();

        game->dropBomb();
        CHECK(game->getGrid().getState(1, 1) == TileState::BOMB);
        CHECK(game->playerHasBomb());

        game->getPlayer().setPosition(at(1, 2));
        game->dropBomb(); // already has one on the grid
        CHECK(game->getGrid().getState(1, 2) == TileState::SAFE);
    }

    TEST_CASE("a bomb cannot be dropped on a wall or after the game is over") {
        auto game = stillGame();
        game->reset();
        game->getPlayer().setPosition(at(3, 1)); // a wall
        game->dropBomb();
        CHECK_FALSE(game->playerHasBomb());

        auto over = stillGame();
        over->dropBomb(); // never started
        CHECK_FALSE(over->playerHasBomb());
    }

    TEST_CASE("the bomb explodes, destroys walls and frees the player to drop another") {
        auto game = stillGame();
        game->reset();
        game->dropBomb();
        stepAway(*game);

        run(*game, GridModel::BOMB_DELAY + 0.2f);

        CHECK_FALSE(game->playerHasBomb());
        CHECK(game->getGrid().getState(3, 1) == TileState::SAFE); // the wall behind (2, 1)
        CHECK(game->getLives() == BomberGame::START_LIVES);
    }

    TEST_CASE("standing in the blast costs a life and respawns the player invincible") {
        auto game = stillGame();
        game->reset();
        game->dropBomb();

        run(*game, GridModel::BOMB_DELAY + 0.1f); // the player stays on the bomb

        CHECK(game->getLives() == BomberGame::START_LIVES - 1);
        CHECK(game->getPlayer().getPosition() == at(1, 1));
        CHECK(game->getPlayer().isInvincible());

        run(*game, 1.0f); // invincibility covers the rest of the fire
        CHECK(game->getLives() == BomberGame::START_LIVES - 1);
    }

    TEST_CASE("an enemy caught in the blast is killed for a reward") {
        auto game = stillGame();
        int score = 0;
        game->onScoreChanged = [&](const int v) { score = v; };
        game->reset();
        game->dropBomb();
        stepAway(*game);
        game->getEnemies()[0].actor.setPosition(at(2, 1));

        run(*game, GridModel::BOMB_DELAY + 0.2f);

        CHECK_FALSE(game->getEnemies()[0].alive);
        CHECK(game->getEnemies()[1].alive);
        CHECK(game->getScore() == BomberGame::REWARD);
        CHECK(score == BomberGame::REWARD);
        CHECK_FALSE(game->isOver());
    }

    TEST_CASE("killing every enemy wins the game") {
        auto game = stillGame();
        int gameOvers = 0;
        game->onGameOver = [&] { ++gameOvers; };
        game->reset();
        game->dropBomb();
        stepAway(*game);
        game->getEnemies()[0].actor.setPosition(at(2, 1));
        game->getEnemies()[1].actor.setPosition(at(1, 2));
        game->getEnemies()[2].actor.setPosition(at(3, 1)); // a wall that the blast destroys

        run(*game, GridModel::BOMB_DELAY + 0.2f);

        CHECK(game->isOver());
        CHECK(gameOvers == 1);
        CHECK(game->getScore() == 3 * BomberGame::REWARD);
    }
}

TEST_SUITE("BomberGame losing") {
    TEST_CASE("touching an enemy costs a life") {
        auto game = stillGame();
        game->reset();
        game->getEnemies()[0].actor.setPosition(game->getPlayer().getPosition() + ax::Vec2(10.0f, 0.0f));

        game->update(FRAME, ax::Vec2::ZERO);

        CHECK(game->getLives() == BomberGame::START_LIVES - 1);
    }

    TEST_CASE("an enemy one tile away is not touching") {
        auto game = stillGame();
        game->reset();
        game->getEnemies()[0].actor.setPosition(at(2, 1));

        game->update(FRAME, ax::Vec2::ZERO);

        CHECK(game->getLives() == BomberGame::START_LIVES);
    }

    TEST_CASE("running out of lives ends the game once") {
        auto game = stillGame();
        int gameOvers = 0;
        game->onGameOver = [&] { ++gameOvers; };
        game->reset();
        game->getEnemies()[0].actor.setPosition(at(1, 1)); // camping on the spawn point

        run(*game, 3.0f);

        CHECK(game->isOver());
        CHECK(game->getLives() == 0);
        CHECK(gameOvers == 1);
    }

    TEST_CASE("the game ends when the time runs out") {
        auto game = stillGame();
        int gameOvers = 0;
        int lastTime = -1;
        game->onGameOver = [&] { ++gameOvers; };
        game->onTimeChanged = [&](const int v) { lastTime = v; };
        game->reset();

        for (int second = 0; second < BomberGame::START_TIME; ++second)
            game->update(1.0f, ax::Vec2::ZERO);

        CHECK(game->isOver());
        CHECK(game->getTime() == 0);
        CHECK(lastTime == 0);
        CHECK(gameOvers == 1);
    }

    TEST_CASE("time counts down one second at a time") {
        auto game = stillGame();
        game->reset();

        game->update(0.5f, ax::Vec2::ZERO);
        CHECK(game->getTime() == BomberGame::START_TIME);

        game->update(0.6f, ax::Vec2::ZERO);
        CHECK(game->getTime() == BomberGame::START_TIME - 1);
    }

    TEST_CASE("nothing happens after the game is over, and reset starts a new one") {
        auto game = stillGame();
        game->reset();
        game->getEnemies()[0].actor.setPosition(at(1, 1));
        run(*game, 3.0f);
        REQUIRE(game->isOver());

        const int time = game->getTime();
        game->update(5.0f, ax::Vec2::ZERO);
        game->dropBomb();
        CHECK(game->getTime() == time);
        CHECK_FALSE(game->playerHasBomb());

        game->reset();
        CHECK_FALSE(game->isOver());
        CHECK(game->getLives() == BomberGame::START_LIVES);
        CHECK(game->getScore() == 0);
    }
}

TEST_SUITE("BomberGame enemies") {
    TEST_CASE("an enemy follows the direction its brain gives, one tile at a time") {
        // Always take the first free direction: from (11, 1) that is up, then back down.
        auto game = std::make_unique<BomberGame>(std::make_unique<RandomWalkBrain>([](size_t) { return size_t{ 0 }; }));
        game->reset();
        game->getPlayer().setPosition(at(5, 9)); // out of the way

        run(*game, 0.6f);

        const auto& enemy = game->getEnemies()[1].actor;
        CHECK(enemy.getPosition().x == doctest::Approx(at(11, 1).x));
        CHECK(enemy.getPosition().y > at(11, 1).y);
        CHECK(enemy.getPosition().y < at(11, 2).y); // already on its way back down
        CHECK(enemy.getDirection() == DIR_DOWN);
    }

    TEST_CASE("roaming enemies always stay on free tiles") {
        BomberGame game; // default random brain
        game.reset();
        game.getPlayer().setPosition(at(5, 9)); // keep the player out of reach

        for (float t = 0.0f; t < 60.0f && !game.isOver(); t += FRAME) {
            game.update(FRAME, ax::Vec2::ZERO);
            game.getPlayer().setPosition(at(5, 9));
            for (const auto& enemy : game.getEnemies()) {
                const auto [col, row] = GridModel::centerCoords(enemy.actor.getPosition());
                REQUIRE(game.getGrid().getState(col, row) == TileState::SAFE);
            }
        }
    }
}
