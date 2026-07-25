#include "IngredientButton.h"
#include  "RoundedRect.h"

USING_NS_AX;

const std::map<Ingredient, std::string> IngredientButton::Textures = {
    { Ingredient::Bread,   "CookBread.png" },
    { Ingredient::Chicken, "CookChicken.png" },
    { Ingredient::Onions,  "CookOnion.png" },
    { Ingredient::Bacon,   "CookBacon.png" },
};

IngredientButton *IngredientButton::create(Ingredient ingredient)
{
    if (auto* pRet = new (std::nothrow) IngredientButton(); pRet && pRet->init(ingredient))
    {
        pRet->autorelease();
        return pRet;
    }
    else
    {
        delete pRet;
        pRet = nullptr;
        return nullptr;
    }
}

bool IngredientButton::init(Ingredient ingredient)
{
    if (!Node::init())
    {
        return false;
    }

    _ingredient = ingredient;
    constexpr Size size(Side, Side);

    const auto square = RoundedRect::create(size, 16.0f, Color4F(0.0f, 0.0f, 0.0f, 0.6f), Color4F::WHITE);
    addChild(square);

    const auto label = Label::createWithTTF(toUpper(IngredientNames.at(ingredient)), "fonts/arial.ttf", 20.0f);
    label->enableBold();
    label->setPosition(size.width / 2.0f, 16.0f);
    addChild(label);

    setContentSize(size);

    return true;
}

ax::Sprite *IngredientButton::createSprite(const float size)
{
    const auto sprite = Sprite::create(Textures.at(_ingredient));
    sprite->setScale(size / sprite->getContentSize().width);
    sprite->setTag(static_cast<int>(_ingredient));
    return sprite;
}

std::string IngredientButton::toUpper(const std::string &text)
{
    std::string result;
    for (const char c : text) {
        result += std::toupper(c);
    }

    return result;
}
