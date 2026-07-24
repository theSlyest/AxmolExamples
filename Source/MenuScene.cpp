#include "MenuScene.h"

#include "GameScene.h"
#include "Constants.h"

USING_NS_AX;

// on "init" you need to initialize your instance
bool MenuScene::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Scene::init() )
    {
        return false;
    }

    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    auto label = Label::createWithTTF("PLAY", "fonts/arial.ttf", ButtonFontSize);
    auto menuItem = MenuItemLabel::create(label, [this](Object* sender) { toGameScene(); });
    auto menu = Menu::create(menuItem, nullptr);
    menu->setPosition(Vec2(origin.x + visibleSize.width / 2.0f, origin.y + visibleSize.height / 2.0f));
    addChild(menu);

    auto title = Label::createWithTTF("WORD GAME", "fonts/arial.ttf", TitleFontSize);
    title->setPosition(Vec2(origin.x + visibleSize.width / 2.0f, origin.y + visibleSize.height - TitleOffset));
    addChild(title);
    
    return true;
}

void MenuScene::toGameScene()
{
    auto scene = GameScene::create();
    _director->pushScene(scene);
}
