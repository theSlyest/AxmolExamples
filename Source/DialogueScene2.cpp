#include "DialogueScene2.h"

#include "Director.h"
#include "EventDispatcher.h"
#include "EventListenerKeyboard.h"
#include "MainMenuScene.h"
#include "VNDialogue.h"
#include "2d/Layer.h"
#include "2d/Transition.h"

USING_NS_AX;

/**
 * Initializer
 */
bool DialogueScene2::init()
{
    if ( !Scene::init() )
        return false;

    const Vec2& origin = Director::getInstance()->getVisibleOrigin();

    // Background color layer
    const auto bg = LayerColor::create(Color4B(0xff, 0x70, 0x70, 0xff));
    addChild(bg);

    // Keyboard listener
    const auto keyListener = EventListenerKeyboard::create();
    keyListener->onKeyPressed = [this](const EventKeyboard::KeyCode key, Event*) { onKeyPressed(key); };
    _eventDispatcher->addEventListenerWithSceneGraphPriority(keyListener, this);

    // Dialogue box
    const auto dialogue = VNDialogue::create(
        "Aki",
        "This is the first scene of our story.\nThe sky is clear, and a new adventure begins.\nLet's see where this path leads us.",
        [this](Object*) { showMainMenu(); });
    dialogue->setPosition(origin + Vec2(VNDialogue::MARGIN, VNDialogue::MARGIN));
    addChild(dialogue);
    
    return true;
}

/**
 * Display main menu scene
 */
void DialogueScene2::showMainMenu() const
{
    const auto scene = MainMenuScene::create();
    _director->replaceScene(TransitionFade::create(1.0f, scene));        
}

/**
 * Keyboard listener on key pressed callback
 */
void DialogueScene2::onKeyPressed(const ax::EventKeyboard::KeyCode key) const
{
    if (key == EventKeyboard::KeyCode::KEY_SPACE || key == EventKeyboard::KeyCode::KEY_ENTER)
        showMainMenu();
}
