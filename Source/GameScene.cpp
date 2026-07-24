#include "GameScene.h"

#include "AudioEngine.h"
#include "Constants.h"

USING_NS_AX;

std::string GameScene::WORDS;

// on "init" you need to initialize your instance
bool GameScene::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Scene::init() )
    {
        return false;
    }

    preloadAudios();

    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    loadWords();
    selectWord();

    _currentCol = 0;
    _currentRow = 0;

    initTiles(origin);
    initKeyboardListener();

    return true;
}

void GameScene::loadWords()
{
    if (!WORDS.empty())
        return;

    FileUtils* fu = FileUtils::getInstance();
    WORDS = fu->getStringFromFile(WORDS_FILE);
    _lastIndex = 1 + WORDS.size() / (COLS + 2);
}

void GameScene::selectWord()
{
    int line = random(0, _lastIndex);
    _word = WORDS.substr((COLS + 2) * line, COLS);
    for (char& ch : _word) {
        ch = std::toupper(ch);
    }
}

void GameScene::initTiles(const Vec2& origin)
{
    const float offsetX = 10.0f;
    const float offsetY = 647.0f;
    const float space = 10.0f;
    for (int r = 0; r < ROWS; ++r) {
        const float y = origin.y + offsetY - r * (LetterTile::SIZE + space);
        std::vector<LetterTile*> row;
        for (int c = 0; c < COLS; ++c) {
            auto tile = LetterTile::create();
            tile->setPosition(origin.x + offsetX + c * (LetterTile::SIZE + space), y);
            addChild(tile);
            row.push_back(tile);
        }
        _tiles.push_back(row);
    }
}

void GameScene::initKeyboardListener()
{
    _keyboardListener = EventListenerKeyboard::create();
    _keyboardListener->onKeyPressed = [this](ax::EventKeyboard::KeyCode key, ax::Event*) { onKeyPressed(key); };
    _eventDispatcher->addEventListenerWithSceneGraphPriority(_keyboardListener, this);
}

void GameScene::onKeyPressed(EventKeyboard::KeyCode key)
{
    if ((key >= EventKeyboard::KeyCode::KEY_A && key <= EventKeyboard::KeyCode::KEY_Z) 
        || ((key >= EventKeyboard::KeyCode::KEY_CAPITAL_A && key <= EventKeyboard::KeyCode::KEY_CAPITAL_Z))) {
        if (_currentCol == COLS)
            return;

        char letter;
        if (key >= EventKeyboard::KeyCode::KEY_A) {
            letter = 'A' + ((char)key - (char)EventKeyboard::KeyCode::KEY_A);
        }
        else {
            letter = 'A' + ((char)key - (char)EventKeyboard::KeyCode::KEY_CAPITAL_A);
        }

         AudioEngine::play2d(SOUND_ADD);
        _tiles[_currentRow][_currentCol++]->setLetter(letter);
    }
    else if (key == EventKeyboard::KeyCode::KEY_BACKSPACE) {
        if (_currentCol == 0)
            return;

        AudioEngine::play2d(SOUND_REMOVE);
        _tiles[_currentRow][--_currentCol]->erase();
    }
    else if (key == EventKeyboard::KeyCode::KEY_ENTER || key == EventKeyboard::KeyCode::KEY_RETURN) {
        if (_currentCol < COLS)
            return;

        AudioEngine::play2d(SOUND_CONFIRM);        
        confirmRow();
    }
}

void GameScene::confirmRow()
{
    const std::string& rowWord = getCurrentWord();
    if (rowWord == _word) {
        for (auto tile : _tiles[_currentRow]) {
            tile->setState(TileState::GREEN);
        }
        gameEnd();
    }

    evaluateRow();

    if (++_currentRow == ROWS)
        gameEnd();
    
    _currentCol = 0;
}

void GameScene::evaluateRow()
{
    std::map<char, int> wordMap;
    for (char ch : _word) {
        wordMap[ch]++;
    }

    const auto& row = _tiles[_currentRow];

    // GREEN pass
    for (int c = 0; c < COLS; ++c) {
        auto tile = row[c];
        const char letter = tile->getLetter();
        if (letter == _word[c]) {
            tile->setState(TileState::GREEN);
            --wordMap[letter];
        }
    }

    // YELLOW pass
    for (int c = 0; c < COLS; ++c) {
        auto tile = row[c];
        if (tile->getState() == TileState::GREEN)
            continue;

        const char letter = tile->getLetter();
        if (_word.find(letter) != std::string::npos && wordMap[letter] > 0) {
            tile->setState(TileState::YELLOW);
            --wordMap[letter];
        }
    }
}

void GameScene::gameEnd()
{
    _eventDispatcher->removeEventListener(_keyboardListener);
    scheduleOnce([this](float dt) { 
        _director->popScene();
    }, 2.0f, "goBack");
}

std::string GameScene::getCurrentWord()
{
    std::string result;
    for (auto tile : _tiles[_currentRow]) {
        result += tile->getLetter();
    }
    return result;
}

void GameScene::preloadAudios()
{
    AudioEngine::preload(SOUND_ADD);
    AudioEngine::preload(SOUND_CONFIRM);
    AudioEngine::preload(SOUND_REMOVE);
}
