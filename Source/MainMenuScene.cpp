#include "MainMenuScene.h"

#include "DialogueScene1.h"
#include "Director.h"
#include "VNButton.h"
#include "VNSeparator.h"
#include "2d/Label.h"
#include "2d/Transition.h"

USING_NS_AX;

/**
 * Initializer
 */
bool MainMenuScene::init()
{
    if ( !Scene::init() )
        return false;

    const Vec2& size = Director::getInstance()->getVisibleSize();
    const Vec2& origin = Director::getInstance()->getVisibleOrigin();

    // Background color layer
    const auto bg = LayerColor::create(Color4B(0, 0x30, 0x40, 0xff));
    addChild(bg);

    // Title
    const auto label = Label::createWithTTF("My Visual Novel", "fonts/arial.ttf", 96.0f);
    label->setPosition(origin.x + size.x / 2.0f, origin.y + size.y / 2.0f + 128.0f);
    this->addChild(label);

    // Separator
    const auto separator = VNSeparator::create(label->getContentSize().width, 32.0f);
    separator->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    separator->setPosition(origin.x + size.x / 2.0f, origin.y + size.y / 2.0f + 56.0f);
    addChild(separator);

    // Button
    const auto button = VNButton::create("Start Game", [this](Object*) { startGame(); });
    button->setPosition(origin.x + size.x / 2.0f, origin.y + size.y / 2.0f - 96.0f);
    addChild(button);
    return true;
}

/**
 * Display Dialogue 1 scene
 */
void MainMenuScene::startGame() const
{
    const auto scene = DialogueScene1::create();
    _director->replaceScene(TransitionFade::create(1.0f, scene));
}
