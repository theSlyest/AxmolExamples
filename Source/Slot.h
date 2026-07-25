#pragma once

#include "axmol.h"
#include "IngredientButton.h"

class Slot : public ax::DrawNode
{
public:
    static constexpr float Radius = 40.0f;

    virtual bool init(Ingredient ingredient);
    
    // implement the "static create()" method manually
    static Slot* create(Ingredient ingredient);

    void fill();
    void empty();
    bool isFull() const { return _isFull; }

    Ingredient getIngredient() const { return _ingredient; }

private:
    Ingredient _ingredient;
    bool _isFull;
};
