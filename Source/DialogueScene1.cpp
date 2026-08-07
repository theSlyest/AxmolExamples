#include "DialogueScene1.h"

#include "DialogueScene2.h"
#include "Director.h"
#include "EventDispatcher.h"
#include "EventListenerKeyboard.h"
#include "VNDialogue.h"
#include "2d/Layer.h"
#include "2d/Transition.h"

USING_NS_AX;

/**
 * Initializer
 */
bool DialogueScene1::init()
{
    if ( !Scene::init() )
        return false;

    const Vec2& origin = Director::getInstance()->getVisibleOrigin();

    // Background color layer
    const auto bg = LayerColor::create(Color4B(0x60, 0x70, 0xff, 0xff));
    addChild(bg);

    // Keyboard listener
    const auto keyListener = EventListenerKeyboard::create();
    keyListener->onKeyPressed = [this](const EventKeyboard::KeyCode key, Event*) { onKeyPressed(key); };
    _eventDispatcher->addEventListenerWithSceneGraphPriority(keyListener, this);

    // Dialogue box
    const auto dialogue = VNDialogue::create(
        "Aki",
        "This is the first scene of our story.\nThe sky is clear, and a new adventure begins.\nLet's see where this path leads us.",
        [this](Object*) { showDialogue2(); });
    dialogue->setPosition(origin + Vec2(VNDialogue::MARGIN, VNDialogue::MARGIN));
    addChild(dialogue);
    
    return true;
}

/**
 * Display Dialogue scene 2
 */
void DialogueScene1::showDialogue2() const
{
    const auto scene = DialogueScene2::create();
    _director->replaceScene(TransitionFade::create(1.0f, scene));    
}

/**
 * Keyboard listener on key pressed callback
 */
void DialogueScene1::onKeyPressed(const EventKeyboard::KeyCode key) const
{
    if (key == EventKeyboard::KeyCode::KEY_SPACE || key == EventKeyboard::KeyCode::KEY_ENTER)
        showDialogue2();
}
