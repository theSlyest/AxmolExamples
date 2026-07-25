#pragma once

#include <unordered_map>

enum class Ingredient { Bread, Chicken, Onions, Bacon, count };

const std::unordered_map<Ingredient, std::string> IngredientNames = {
    { Ingredient::Bread,   "Bread" },
    { Ingredient::Chicken, "Chicken" },
    { Ingredient::Onions,  "Onions" },
    { Ingredient::Bacon,   "Bacon" },
};
