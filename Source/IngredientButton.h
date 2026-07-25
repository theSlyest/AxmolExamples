#pragma once

#include "2d/Node.h"
#include "2d/Sprite.h"
#include "Ingredient.h"

class IngredientButton : public ax::Node
{
public:
    static constexpr float Side = 156.0f;

    virtual bool init(Ingredient ingredient);
    
    static IngredientButton* create(Ingredient ingredient);

    ax::Sprite* createSprite(float size);

    std::string getIngredientName() const { return IngredientNames.at(_ingredient); };

private:
    static const std::map<Ingredient, std::string> Textures;

    Ingredient _ingredient = Ingredient::Bread;
    ax::Sprite* _sprite = nullptr;

    static std::string toUpper(const std::string& text);
};
