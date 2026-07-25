#pragma once

#include "axmol.h"

constexpr float BASE_LENGTH = 40.0f;
constexpr int GRID_WIDTH = 24;
constexpr int GRID_HEIGHT = 16;

enum class BoxType { Player, Obstacle, SafeZone };

class BaseBox : public ax::DrawNode
{
public:
    virtual bool init(BoxType type, int width, int height);
    static BaseBox* create(BoxType type, int width, int height);

    void setDiscretePosition(int x, int y);
    int getDiscretePositionX() const;
    int getDiscretePositionY() const;
    int getDiscreteWidth() const;

private:
    BoxType _boxType = BoxType::SafeZone;

    static float convertDiscreteValue(int val);
    static ax::Color4F typeColor(BoxType type);

    void drawBox(ax::Color4F color, float width, float height);
};
