#ifndef __GAME_SCENE_H__
#define __GAME_SCENE_H__

#include "axmol.h"
#include "LetterTile.h"

class GameScene : public ax::Scene
{
public:
    virtual bool init();
    
    // implement the "static create()" method manually
    CREATE_FUNC(GameScene);

private:
    static constexpr int ROWS = 6;
    static constexpr int COLS = 5;
    static std::string WORDS;

    std::string _word;
    int _lastIndex;
    int _currentCol;
    int _currentRow;

    std::vector<std::vector<LetterTile*>> _tiles;

    ax::EventListenerKeyboard* _keyboardListener;

    void loadWords();
    void selectWord();

    void initTiles(const ax::Vec2& origin);
    void initKeyboardListener();

    void onKeyPressed(ax::EventKeyboard::KeyCode key);

    void confirmRow();
    void evaluateRow();

    void gameEnd();

    std::string getCurrentWord();

    void preloadAudios();
};

#endif // __GAME_SCENE_H__
