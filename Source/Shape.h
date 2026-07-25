#pragma once

#include "axmol.h"

enum class ShapeType {
    Circle,
    Diamond,
    Heart,
    Hexagon,
    Oval,
    Pentagon,
    Rectangle,
    Square,
    Star,
    Triangle,
    count
};

class Shape : public ax::DrawNode
{
public:
    virtual bool init(ShapeType type, bool isShadow);

    ShapeType getType() const { return _type; }

    bool isTouchable() const { return _isTouchable; }
    void setTouchable(const bool touchable) { _isTouchable = touchable; }

    void highlight();

    void update(float dt) override;

    ax::Vec2 getCenter() const;

protected:
    static const ax::Color4F ShadowColor;
    static constexpr float Thickness = 3.0f;

    bool _isShadow = false;
    bool _isTouchable = false;
    float _elapsed = 0.0f;
    ShapeType _type = ShapeType::Circle;
        
    ax::Color4F _fillColor;
    ax::Color4F _borderColor;

    ax::Color4F getFillColor();
    ax::Color4F getBorderColor() const;

    virtual ax::Color4F getShapeColor() = 0;

    virtual void drawType() = 0;
};
