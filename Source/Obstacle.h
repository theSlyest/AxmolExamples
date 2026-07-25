#pragma once

#include "BaseBox.h"

constexpr int LANE_HEIGHT = 1;
constexpr int MIN_WIDTH = 1;
constexpr int MAX_WIDTH = 4;
constexpr int MIN_SPEED = 1;
constexpr int MAX_SPEED = 3;

class Obstacle : public BaseBox
{
public:
    virtual bool init(int lane);
    static Obstacle* create(int lane);

    int getLane() const;

private:
    bool _goLeft = false;
    int _lane = 0;

    static int getPeriodFromLane(int lane);
    void move();
};
