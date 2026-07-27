#pragma once

#include "axmol.h"

class RushHourVehicle : public ax::DrawNode
{
public:
    virtual bool init(const ax::Color4F& color, bool isTruck, bool vertical);
    
    // implement the "static create()" method manually
    static RushHourVehicle* create(const ax::Color4F& color, bool isTruck = false, bool vertical = false);

    int getSize() const { return _size; }
    bool isVertical() const { return _isVertical; }

protected:
    static constexpr float RADIUS = 8.0f;
    static constexpr float PADDING = 4.0f;
    static constexpr float CELL_SIZE = 70.0f;

    int _size = 0;
    bool _isVertical = false;

    void drawRoundedRect(const ax::Vec2& origin, const ax::Vec2& destination, float radius, const ax::Color4F& color);
};
