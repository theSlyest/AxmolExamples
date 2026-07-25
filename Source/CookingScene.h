#pragma once

#include "axmol.h"
#include "FryingPan.h"
#include "Ingredient.h"
#include "IngredientButton.h"
#include "RoundedRect.h"
#include "Slot.h"

struct CookingState { float duration{}; ax::Color3B color; };

class CookingScene : public ax::Scene
{
public:
    bool init() override;
    
    // implement the "static create()" method manually
    CREATE_FUNC(CookingScene);

    void update(float dt) override;

private:
    static const std::map<Ingredient, std::vector<CookingState>> CookingStates;

    int _points = 0;
    int _successful = 0;
    int _attempts = 0;
    int _cookingLevel = 0;
    float _timer = 0;
    float _btnSpriteY = 0;
    bool _isCooking = false;

    ax::Vec2 _touchOffset;
    ax::Sprite* _draggedSprite = nullptr;
    ax::Sprite* _cookedSprite = nullptr;
    FryingPan* _pan = nullptr;
    RoundedRect* _timerPanel = nullptr;
    RoundedRect* _resultPanel = nullptr;
    std::vector<IngredientButton*> _ingredientButtons;
    std::vector<ax::Sprite*> _ingredientSprites;
    std::vector<Slot*> _slots;

    ax::Label* _successfulLabel = nullptr;
    ax::Label* _attemptsLabel = nullptr;
    ax::Label* _timerLabel = nullptr;
    ax::Label* _resultLabel = nullptr;

    ax::EventListenerTouchOneByOne* _touchListener = nullptr;

    void initSuccessful(const ax::Size& visibleSize, const ax::Vec2& origin);
    void initAttempts(const ax::Size& visibleSize, const ax::Vec2& origin);
    void initTimer(const ax::Size& visibleSize, const ax::Vec2& origin);
    void initFryingPan(const ax::Size& visibleSize, const ax::Vec2& origin);
    void initIngredients(const ax::Size& visibleSize, const ax::Vec2& origin);
    void initResult(const ax::Size& visibleSize, const ax::Vec2& origin);

    void initTouchListener();
    bool onTouchBegan(const ax::Touch* touch);
    void onTouchMoved(ax::Touch* touch);
    void onTouchEnded();

    void setSuccessful(int successful);
    void setAttempts(int attempts);
    void updateTimer(float dt);

    void startCooking();
    void pauseCooking();
    void resumeCooking();
    void stopCooking();
    bool checkAllCooked() const;
    void displayResult();

    void reset();
};
