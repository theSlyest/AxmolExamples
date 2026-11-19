#include "ActorModel.h"

bool ActorModel::hasReachedDestination(const ax::Vec2& nextPos) const
{
    if (_direction.x > 0.0f && nextPos.x >= _destination.x)
        return true;

    if (_direction.x < 0.0f && nextPos.x <= _destination.x)
        return true;

    if (_direction.y > 0.0f && nextPos.y >= _destination.y)
        return true;

    if (_direction.y < 0.0f && nextPos.y <= _destination.y)
        return true;

    return false;
}

void ActorModel::update(const float dt)
{
    if (_remaining > 0.0f)
        _remaining -= dt;
}
