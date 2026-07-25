#pragma once


#include "axmol.h"
#include "ConfettiNode.h"
#include "Shape.h"

class ShadowShapeScene : public ax::Scene
{
public:
    virtual bool init();

    // implement the "static create()" method manually
    CREATE_FUNC(ShadowShapeScene);

private:
    static constexpr float ShadowScale = 1.5f;

    int _matchCount = 0;
    int _score = 0;
    bool _previousFail = false;

    ax::Label* _scoreLabel = nullptr;
    ax::Label* _msgLabel = nullptr;
    ax::Label* _winLabel = nullptr;
    ax::EventListenerKeyboard* _keyListener = nullptr;
    ax::EventListenerTouchOneByOne* _touchListener = nullptr;
    ax::Vec2 _touchOffset;
    ax::Vec2 _movedOriginalPos;

    Shape* _movedShape = nullptr;
    std::vector<Shape*> _shapes;
    std::vector<Shape*> _shadows;
    std::unordered_map<ShapeType, std::function<Shape*()>> _shadowGenerators;
    ConfettiNode* _confetti = nullptr;

    void initScore(const ax::Size& visibleSize, const ax::Vec2& origin);
    void initReset(const ax::Size& visibleSize, const ax::Vec2& origin);
    void initWin(const ax::Size& visibleSize, const ax::Vec2& origin);
    void initTitle(const ax::Size& visibleSize, const ax::Vec2& origin);
    void initMessage(const ax::Size& visibleSize, const ax::Vec2& origin);
    void initBottomLayer(const ax::Size &visibleSize, const ax::Vec2& origin);
    void initShapes();
    void initShadows();
    void initConfetti();
    void initKeyboardListener();
    void initTouchListener();

    void addShape(Shape* shape);
    void resetShapes();
    void resetShadows();
    void softReset();
    void hardReset();

    void setScore(int score);
    void setMessage(const std::string& msg) const;
    void displayMessage(const std::string& msg);
    void celebrate();

    void onKeyPressed(ax::EventKeyboard::KeyCode key);
    bool onTouchBegan(const ax::Touch* touch);
    void onTouchMoved(const ax::Touch* touch) const;
    void onTouchEnded();
};
