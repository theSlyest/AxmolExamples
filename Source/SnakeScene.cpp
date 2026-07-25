#include "SnakeScene.h"

USING_NS_AX;

// on "init" you need to initialize your instance
bool SnakeScene::init()
{
    if (!Scene::init())
        return false;

    const auto& visibleSize = Director::getInstance()->getVisibleSize();
    const Vec2& origin = Director::getInstance()->getVisibleOrigin();

    _scoreLabel = Label::createWithTTF("Score: 0", "fonts/arial.ttf", 24);
    _scoreLabel->setColor(Color3B::WHITE);
    _scoreLabel->setAnchorPoint(Vec2(0.0f, 1.0f));
    // position the label on the center of the screen
    _scoreLabel->setPosition(Vec2(origin.x + 10.0f, origin.y + visibleSize.height - 10.0f));

    // add the label as a child to this layer
    addChild(_scoreLabel, 1);

    _gameOverLayer = Layer::create();
    _gameOverLayer->setVisible(false);
    addChild(_gameOverLayer, 2);

    Label* gameOverLabel = Label::createWithTTF("GAME OVER", "fonts/arial.ttf", 48.0f);
    gameOverLabel->setColor(Color3B::RED);
    gameOverLabel->setPosition(Vec2(origin.x + visibleSize.width / 2.0f, origin.y + visibleSize.height / 2.0f));
    _gameOverLayer->addChild(gameOverLabel);

    Label* restartLabel = Label::createWithTTF("PRESS R TO RESTART", "fonts/arial.ttf", 24.0f);
    restartLabel->setColor(Color3B::WHITE);
    restartLabel->setPosition(Vec2(origin.x + visibleSize.width / 2.0f, origin.y + visibleSize.height / 2.0f - 50.0f));
    _gameOverLayer->addChild(restartLabel);

    _grid = Grid::create();
    addChild(_grid);

    _keyboardListener = EventListenerKeyboard::create();
    _keyboardListener->onKeyPressed = [this](const EventKeyboard::KeyCode keyCode, Event *)
    {
        SnakeDirection newDirection;
        switch (keyCode)
        {
        case EventKeyboard::KeyCode::KEY_UP_ARROW:
            newDirection = {1, 0};
            break;
        case EventKeyboard::KeyCode::KEY_DOWN_ARROW:
            newDirection = {-1, 0};
            break;
        case EventKeyboard::KeyCode::KEY_LEFT_ARROW:
            newDirection = {0, -1};
            break;
        case EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
            newDirection = {0, 1};
            break;
        case EventKeyboard::KeyCode::KEY_R:
            if (_gameOverLayer->isVisible())
            {
                restartGame();
                return;
            }
        default:
            return; // Ignore other keys
        }
                    
        if (!isGameOver())
        {
            _grid->setDirection(newDirection);
        }
    };
    _eventDispatcher->addEventListenerWithSceneGraphPriority(_keyboardListener, this);

    startGame();

    return true;
}

void SnakeScene::startGame()
{
    _grid->setup();
    schedule([this](const float dt)
    {
        if (const char result = _grid->updateSnake(); result == -1) // Collision with itself
        {
            unschedule("updateSnake");
            _gameOverLayer->setVisible(true);
        }
        else if (result == 1) // Ate food
        {
            _scoreLabel->setString("Score: " + std::to_string(_grid->getScore()));
        }
    }, 0.15f, AX_REPEAT_FOREVER, 0.15f, "updateSnake"); // Schedule snake movement every 0.15 seconds
}

void SnakeScene::restartGame()
{
    _grid->resetGame();
    _scoreLabel->setString("Score: 0");
    _gameOverLayer->setVisible(false);
    startGame();
}
