#include "RippleControls.h"

#include "EventDispatcher.h"
#include "EventListenerKeyboard.h"

USING_NS_AX;

bool RippleControls::init()
{
    if (!Node::init())
        return false;

    _touchListener = EventListenerTouchOneByOne::create();
    _touchListener->onTouchBegan = [this](Touch* touch, Event*) { return onTouch && onTouch(touch); };
    _eventDispatcher->addEventListenerWithSceneGraphPriority(_touchListener, this);

    const auto keyboardListener = EventListenerKeyboard::create();
    keyboardListener->onKeyPressed = [this](const EventKeyboard::KeyCode key, Event*) {
        if (key == EventKeyboard::KeyCode::KEY_R && onRestart)
            onRestart();
    };
    _eventDispatcher->addEventListenerWithSceneGraphPriority(keyboardListener, this);

    return true;
}

void RippleControls::setTouchEnabled(const bool enabled) const
{
    _touchListener->setEnabled(enabled);
}
