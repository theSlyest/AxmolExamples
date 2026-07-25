#pragma once

#include "axmol.h"

constexpr float SQUARE_SIZE = 32.0f;

enum class SquareType {
    Start,
    Target,
    Empty,
    Wall,
    Path
};

class Square : public ax::DrawNode
{
public:
    static Square* create(SquareType type);

    virtual bool init(SquareType type);
    
    SquareType getType() const { return _type; }
    void setType(SquareType type);

    void switchType();

private:
    static const std::unordered_map<SquareType, ax::Color4F> Colors;

    SquareType _type = SquareType::Empty;
};
