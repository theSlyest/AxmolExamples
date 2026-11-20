#include "SignalControls.h"

#include <algorithm>

#include "EventDispatcher.h"

USING_NS_AX;

bool SignalControls::init()
{
    if (!Node::init())
        return false;

    const auto listener = EventListenerKeyboard::create();
    listener->onKeyPressed = [this](const EventKeyboard::KeyCode key, Event*) { onKeyPressed(key); };
    listener->onKeyReleased = [this](const EventKeyboard::KeyCode key, Event*) { onKeyReleased(key); };
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

    return true;
}

int SignalControls::getDirection() const
{
    // The most recently pressed key that is still held decides.
    return _held.empty() ? 0 : directionOf(_held.back());
}

int SignalControls::directionOf(const EventKeyboard::KeyCode key)
{
    switch (key) {
        case EventKeyboard::KeyCode::KEY_LEFT_ARROW:
        case EventKeyboard::KeyCode::KEY_A:
            return -1;

        case EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
        case EventKeyboard::KeyCode::KEY_D:
            return 1;

        default:
            return 0;
    }
}

void SignalControls::onKeyPressed(const EventKeyboard::KeyCode key)
{
    if (directionOf(key) != 0) {
        // Key repeat sends further presses of a held key: keep the original order.
        if (std::ranges::find(_held, key) == _held.end())
            _held.push_back(key);
    }
    else if (key == EventKeyboard::KeyCode::KEY_R && onRestart)
        onRestart();
}

void SignalControls::onKeyReleased(const EventKeyboard::KeyCode key)
{
    std::erase(_held, key);
}
