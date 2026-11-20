#include "SignalDial.h"

#include <algorithm>

#include "fmt/format.h"

USING_NS_AX;

bool SignalDial::init()
{
    if (!DrawNode::init())
        return false;

    setContentSize(Size(DIAL_WIDTH, DIAL_HEIGHT));
    const Color4F color(COLOR);
    constexpr float spacing = DIAL_WIDTH / 40;
    int value = 0;
    for (int i = 0; i < 41; ++i) {
        const float x = static_cast<float>(i) * spacing;
        if (i % 4 == 0) {
            drawSolidRect(Vec2(x - LARGE_WIDTH, DIAL_HEIGHT - RULER_Y_OFFSET), 
                          Vec2(x + LARGE_WIDTH, DIAL_HEIGHT - RULER_HEIGHT - RULER_Y_OFFSET), 
                          color);
            const auto label = Label::createWithTTF(std::to_string(value), "fonts/arial.ttf", RULER_FONT);
            label->setTextColor(COLOR);
            label->setPosition(x, LABEL_Y);
            addChild(label);
            value += 10;
        }
        else
            drawSolidRect(Vec2(x - SMALL_WIDTH, DIAL_HEIGHT - RULER_HEIGHT * 0.25f - RULER_Y_OFFSET), 
                          Vec2(x + SMALL_WIDTH, DIAL_HEIGHT - RULER_HEIGHT * 0.75f - RULER_Y_OFFSET), 
                          color);
    }
    
    initArrow();

    return true;
}

void SignalDial::initArrow()
{
    const Color4F color(COLOR);
    _arrow = DrawNode::create();
    _arrow->setContentSize(Size(ARROW_WIDTH, ARROW_HEIGHT));
    _arrow->drawRect(Vec2::ZERO, Vec2(ARROW_WIDTH, ARROW_RECT_HEIGHT), color, ARROW_RECT_LINE);
    _arrow->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    _arrow->setPosition(DIAL_WIDTH / 2.0f, 0.0f);
    addChild(_arrow);

    _lockBar = LayerColor::create(Color4B(0x46, 0xd9, 0xa8, 255), ARROW_WIDTH, ARROW_RECT_HEIGHT);
    _lockBar->setScaleX(0.0f);
    _arrow->addChild(_lockBar, -1);

    _label = Label::createWithTTF("TUNE 050", "fonts/arial.ttf", ARROW_FONT);
    _label->setTextColor(COLOR);
    _label->setPosition(ARROW_WIDTH / 2.0f, ARROW_RECT_HEIGHT / 2.0f);
    _arrow->addChild(_label);

    constexpr float height = DIAL_HEIGHT - ARROW_RECT_HEIGHT;
    constexpr float width = ARROW_THICKNESS * 2.0f;
    constexpr float offset = ARROW_THICKNESS * 0.5f;
    constexpr float length = height * 0.8f;
    constexpr Vec2 points[7] {
        Vec2(offset, length),
        Vec2(offset, 0.0f),
        Vec2(width - offset, 0.0f),
        Vec2(width - offset, length),
        Vec2(width, length),
        Vec2(width * 0.5f, height),
        Vec2(0.0f, length),
    };
    const auto draw = DrawNode::create();
    draw->setContentSize(Size(width, height));
    draw->drawSolidPoly(points, 7, color);
    draw->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    draw->setPosition(ARROW_WIDTH / 2.0f, ARROW_RECT_HEIGHT);
    _arrow->addChild(draw);
}

void SignalDial::setValue(const float value)
{
    const float clamped = std::clamp(value, 0.0f, 1.0f);
    _arrow->setPositionX(clamped * DIAL_WIDTH);

    // Rebuilding the label text every frame is wasteful: only do it when the number changes.
    if (const int percent = static_cast<int>(clamped * 100); percent != _shownPercent) {
        _shownPercent = percent;
        _label->setString(fmt::format("TUNE {:03d}", percent));
    }
}
