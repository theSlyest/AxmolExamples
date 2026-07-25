#include "Shape.h"

USING_NS_AX;

const Color4F Shape::ShadowColor(0.3f, 0.3f, 0.3f, 1.0f);

bool Shape::init(const ShapeType type, const bool isShadow)
{
    if ( !DrawNode::init() )
        return false;

    _type = type;
    _isShadow = isShadow;
    _isTouchable = !isShadow;

    _fillColor = getFillColor();
    _borderColor = getBorderColor();

    drawType();

    return true;
}

Color4F Shape::getFillColor() { return _isShadow ? ShadowColor : getShapeColor(); }

Color4F Shape::getBorderColor() const { return _isShadow ? Color4F(0.2f, 0.2f, 0.2f, 1.0f) : Color4F::WHITE; }

void Shape::highlight()
{
    _elapsed = 0.0f;
    scheduleUpdate();
}

Vec2 Shape::getCenter() const
{
    const ax::Rect& box = getBoundingBox();
    return {box.getMidX(), box.getMidY()};
}

void Shape::update(const float dt)
{
    _elapsed += dt;
    if (_elapsed >= 1.0f) {
        _fillColor = getFillColor();
        _borderColor = getBorderColor();
        unscheduleUpdate();
    }
    else {
        const float dColor = 0.5f - abs(0.5f - _elapsed);
        const Color4F delta(dColor, dColor, dColor, 1.0f);
        _fillColor = getFillColor() + delta;
        _borderColor = getBorderColor() + delta;
    }

    clear();
    drawType();
}
