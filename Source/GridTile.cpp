#include "GridTile.h"

USING_NS_AX;

const std::map<TileState, Color4F> GridTile::COLORS {
    { TileState::SAFE, Color4F(0.0f, 0.0f, 0.0f, 1.0f) },
    { TileState::WALL, Color4F(0.60f, 0.40f, 0.05f, 1.0f) },
    { TileState::STEEL, Color4F(0.27f, 0.27f, 0.34f, 1.0f) },
};

const Color4F GridTile::BORDER_COLOR(0.73f, 0.73f, 0.66f, 1.0f);
const Color4F GridTile::FIRE_COLOR(0.92f, 0.34f, 0.29f, 1.0f);

bool GridTile::init()
{
    if (!DrawNode::init())
        return false;

    setContentSize(Size(SIZE, SIZE));

    const auto border = DrawNode::create();
    border->drawRect(Vec2::ZERO, Vec2(_contentSize), BORDER_COLOR, 1.0f);
    addChild(border);

    constexpr Vec2 offset(3.0f, 3.0f);
    _fire = DrawNode::create();
    _fire->setContentSize(_contentSize);
    _fire->drawSolidRect(offset, Vec2(_contentSize) - offset, FIRE_COLOR);
    _fire->setVisible(false);
    addChild(_fire);

    return true;
}

void GridTile::show(const TileState state, const bool onFire)
{
    // A tile with a bomb looks like an empty tile; the bomb is a separate node.
    const TileState drawn = state == TileState::BOMB ? TileState::SAFE : state;
    if (!_shown || drawn != _state) {
        _shown = true;
        _state = drawn;
        clear();
        drawSolidRect(Vec2::ZERO, Vec2(_contentSize), COLORS.at(drawn));
    }

    _fire->setVisible(onFire);
}
