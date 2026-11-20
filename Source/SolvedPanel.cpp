#include "SolvedPanel.h"

#include "2d/Menu.h"

USING_NS_AX;

const Color4F SolvedPanel::BG_COLOR(0.06f, 0.1f, 0.13f, 1.0f);
const Color4F SolvedPanel::BORDER_COLOR(0.9f, 0.49f, 0.14f, 1.0f);
const Size SolvedPanel::SIZE(360.0f, 240.0f);

bool SolvedPanel::init()
{
    if (!DrawNode::init())
        return false;

    setContentSize(SIZE);

    drawSolidRect(Vec2::ZERO, Vec2(SIZE), BG_COLOR);
    constexpr float offset = BORDER / 2.0f;
    drawRect(Vec2(offset, offset), Vec2(SIZE.width - offset, SIZE.height - offset), BORDER_COLOR, BORDER);

    _mirrorLbl = Label::createWithTTF("Mirrors used: ", "fonts/arial.ttf", 24.0f);
    _mirrorLbl->setTextColor(Color4B(92, 169, 79, 255));
    _mirrorLbl->setPosition(SIZE.width / 2.0f, SIZE.height * 3.0f / 4.0f);
    addChild(_mirrorLbl);

    _bestLbl = Label::createWithTTF("Best: ", "fonts/arial.ttf", 20.0f);
    _bestLbl->setTextColor(Color4B(241, 196, 15, 255));
    _bestLbl->setPosition(SIZE.width / 2.0f, SIZE.height / 2.0f);
    addChild(_bestLbl);

    const auto label  = Label::createWithTTF("Play again", "fonts/arial.ttf", 20.0f);
    const Vec2 btnSize = label->getContentSize() + Size(48.0f, 24.0f);

    drawRect(Vec2((SIZE.width - btnSize.width) / 2.0f, SIZE.height / 4.0f - btnSize.height / 2.0f),
            Vec2((SIZE.width + btnSize.width) / 2.0f, SIZE.height / 4.0f + btnSize.height / 2.0f),
            Color4F(0.21f, 0.6f, 0.86f, 1.0f), BORDER);

    const auto menuItem = MenuItemLabel::create(label, [this] (Object* sender) { menuItemCallback(sender); });
    const auto menu = Menu::create(menuItem, nullptr);
    menu->setPosition(SIZE.width / 2.0f, SIZE.height / 4.0f);
    addChild(menu);
    
    return true;
}

void SolvedPanel::setValues(const std::string &mirrors, const std::string &best) const
{
    _mirrorLbl->setString("Mirrors used: " + mirrors);
    _bestLbl->setString("Best: " + best);
}

void SolvedPanel::menuItemCallback(Object *sender) const
{
    if (_btnCallback != nullptr)
        _btnCallback(sender);
}
