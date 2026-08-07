#include "VNButton.h"

#include "2d/Label.h"

USING_NS_AX;

Color4F VNButton::BG_COLOR(0.35f, 0.35f, 0.35f, 1.0f);
Color4F VNButton::BORDER_COLOR(0.65f, 0.65f, 0.65f, 1.0f);
Size VNButton::PADDING(96.0f, 32.0f);

/**
 * Factory method (copied from engine CREATE_FUNC macro)
 */
VNButton *VNButton::create(const std::string &text, const ax::ccMenuCallback &callback)
{
    if (auto pRet = new (std::nothrow) VNButton(); pRet && pRet->init(text, callback))
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

bool VNButton::init(const std::string &text, const ax::ccMenuCallback &callback)
{
    // Label for the button text
    const auto label = Label::createWithTTF(text, "fonts/arial.ttf", FONT_SIZE);

    // Menu item
    auto item = MenuItemLabel::create(label, callback);
    const Size& lblSize = label->getContentSize();
    const Size size(lblSize + PADDING);
    item->setContentSize(size);
    label->setPosition((size - lblSize) / 2.0f);
    
    // Background rectangle
    const auto bg = DrawNode::create();
    bg->drawSolidRect(Vec2::ZERO, size, BG_COLOR);
    bg->drawRect(Vec2::ZERO, size, BORDER_COLOR);
    bg->setContentSize(size);
    item->addChild(bg, -1);

    if (!Menu::initWithArray({ item }))
        return false;

    setContentSize(size);

    return true;
}
