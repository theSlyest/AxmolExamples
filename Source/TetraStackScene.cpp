#include "TetraStackScene.h"

USING_NS_AX;

bool TetraStackScene::init()
{
    if ( !Scene::init() )
    {
        return false;
    }

    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    auto bg = LayerColor::create(Color4B(0x0c, 0x10, 0x18, 0xff));
    addChild(bg);

    auto title = Label::createWithTTF("TETRASTACK", "fonts/arial.ttf", 32.0f);
    title->setTextColor(Color4B(0xff, 0xc2, 0x4a, 0xff));
    title->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
    title->setPosition(Vec2(origin.x + 20.0f, origin.y + visibleSize.height - 20.0f));
    addChild(title);

    auto help = Label::createWithTTF("Arrow keys to move and rotate, Space to hard drop, R to restart", "fonts/arial.ttf", 16.0f);
    help->setPosition(origin.x + 20.0f, origin.y + 50.0f);

    initScore(origin);
    initLines(origin);
    initGrid(origin);
    initNextBox(origin);
    initResultLayer(visibleSize);
    initKeyboardListener();

    _eventDispatcher->addCustomEventListener(TetraGrid::LOCK_EVENT, [this](EventCustom* event) {
        onBlockLocked(static_cast<LinesData*>(event->getUserData()));
    });

    reset();

    return true;
}

void TetraStackScene::initScore(const Vec2 &origin)
{
    _score = 0;
    _scoreLbl = Label::createWithTTF("Score: 0", "fonts/arial.ttf", 24.0f);
    _scoreLbl->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
    _scoreLbl->setPosition(origin.x + HUD_OFFSET, origin.y + 540.0f);
    addChild(_scoreLbl);
}

void TetraStackScene::initLines(const Vec2 &origin)
{
    _linesCount = 0;
    _linesLbl = Label::createWithTTF("Lines: 0", "fonts/arial.ttf", 24.0f);
    _linesLbl->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
    _linesLbl->setPosition(origin.x + HUD_OFFSET, origin.y + 500.0f);
    addChild(_linesLbl);
}

void TetraStackScene::initGrid(const Vec2 &origin)
{
    _grid = TetraGrid::create();
    _grid->setPosition(origin.x + 20.0f, origin.y + 100.0f);
    addChild(_grid);
}

void TetraStackScene::initNextBox(const ax::Vec2 &origin)
{
    auto label = Label::createWithTTF("Next", "fonts/arial.ttf", 24.0f);
    label->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
    label->setPosition(origin.x + HUD_OFFSET, origin.y + 420.0f);
    addChild(label);

    float size = 120.0f;
    _nextBoxSize = Size(size, size);
    auto square = DrawNode::create();
    square->drawRect(Vec2::ZERO, Vec2(size, size), TetraGrid::COLOR, TetraGrid::BORDER);
    square->setPosition(origin.x + HUD_OFFSET, origin.y + 290.0f);
    addChild(square);

    for (int i = 0; i < 4; ++i) {
        auto cell = TetraCell::create(Tetromino::I);
        square->addChild(cell);
        _nextCells.push_back(cell);
    }
}

void TetraStackScene::initResultLayer(const Size &size)
{
    _resultLayer = LayerColor::create(Color4B(0, 0, 0, 192));
    _resultLayer->setVisible(false);
    addChild(_resultLayer);

    auto gameOver = Label::createWithTTF("GAME OVER", "fonts/arial.ttf", 32.0f);
    gameOver->setPosition(size.width / 2.0f, size.height / 2.0f + 40.0f);
    _resultLayer->addChild(gameOver);

    _endScoreLbl = Label::createWithTTF("Score: 0", "fonts/arial.ttf", 24.0f);
    _endScoreLbl->setPosition(size.width / 2.0f, size.height / 2.0f);
    _resultLayer->addChild(_endScoreLbl);

    auto restart = Label::createWithTTF("Press R to restart", "fonts/arial.ttf", 16.0f);
    restart->setPosition(size.width / 2.0f, size.height / 2.0f - 40.0f);
    _resultLayer->addChild(restart);
}

void TetraStackScene::initKeyboardListener()
{
    _keyboardListener = EventListenerKeyboard::create();
    _keyboardListener->onKeyPressed = [this](EventKeyboard::KeyCode key, Event *) { onKeyPressed(key); };
    _keyboardListener->onKeyReleased = [this](EventKeyboard::KeyCode key, Event *) { onKeyReleased(key); };
    _eventDispatcher->addEventListenerWithSceneGraphPriority(_keyboardListener, this);
}

void TetraStackScene::onKeyPressed(EventKeyboard::KeyCode key)
{
    switch (key) {
        case EventKeyboard::KeyCode::KEY_R:
            reset();
            break;

        case EventKeyboard::KeyCode::KEY_SPACE:
            if (!_isPlaying)
                return;

            _grid->dropBlock();
            break;

        case EventKeyboard::KeyCode::KEY_UP_ARROW:
            if (!_isPlaying)
                return;
                
            _grid->turnBlock();
            break;

        case EventKeyboard::KeyCode::KEY_DOWN_ARROW:
            if (!_isPlaying)
                return;
                
            _grid->speedUpBlock(true);
            break;

        case EventKeyboard::KeyCode::KEY_LEFT_ARROW:
            if (!_isPlaying)
                return;
                
            _grid->shiftBlock(false);
            break;

        case EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
            if (!_isPlaying)
                return;
                
            _grid->shiftBlock(true);
            break;
    }
}

void TetraStackScene::onKeyReleased(ax::EventKeyboard::KeyCode key)
{
    if (_isPlaying && key == EventKeyboard::KeyCode::KEY_DOWN_ARROW) {
        _grid->speedUpBlock(false);
    }
}

void TetraStackScene::onBlockLocked(LinesData* data)
{
    const int lines = data->number;
    if (lines > 0) {
        setLines(_linesCount + lines);
        const int points = lines == 4 ? 800 : (100 + (lines - 1) * 200);
        setScore(_score + points);
    }
    delete data;
    pushNextBlock();
}

void TetraStackScene::setScore(int score)
{
    _score = score;
    _scoreLbl->setString("Score: " + std::to_string(score));
}

void TetraStackScene::setLines(int lines)
{
    _linesCount = lines;
    _linesLbl->setString("Lines: " + std::to_string(lines));
}

void TetraStackScene::reset()
{
    setScore(0);
    setLines(0);
    _nextMinos = std::queue<Tetromino>();
    _nextMinos.push(Tetromino::T);
    _nextMinos.push(Tetromino::I);
    _grid->reset();
    _isPlaying = true;
    _resultLayer->setVisible(false);
    pushNextBlock();
}

void TetraStackScene::pushNextBlock()
{
    Tetromino mino = _nextMinos.front();
    _nextMinos.pop();
    _nextMinos.push((Tetromino)random(0, 6));
    showNextBlock();
    if (!_grid->pushBlock(mino)) {
        showResult();
    }
}

void TetraStackScene::showNextBlock()
{
    const Tetromino mino = _nextMinos.front();
    const auto& shape = TetraGrid::SHAPES.at(mino);
    const int rows = shape.size();
    const int cols = shape[0].size();
    const float offsetX = (_nextBoxSize.width - cols * TetraCell::SIZE) / 2.0f;
    const float offsetY = (_nextBoxSize.height - rows * TetraCell::SIZE) / 2.0f;
    int i = 0;
    for (int r = 0; r < rows; ++r) {
        const float y = offsetY + (rows - r - 1) * TetraCell::SIZE;
        for (int c = 0; c < cols; ++c) {
            if (shape[r][c]) {
                _nextCells[i]->setTetromino(mino);
                _nextCells[i++]->setPosition(offsetX + c * TetraCell::SIZE, y);
            }
        }
    }

}

void TetraStackScene::showResult()
{
    _isPlaying = false;
    _grid->speedUpBlock(false);
    _endScoreLbl->setString(_scoreLbl->getString());
    _resultLayer->setVisible(true);
}
