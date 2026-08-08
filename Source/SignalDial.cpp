#include "SignalDial.h"

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
        const float x = i * spacing;
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

float SignalDial::tuneBy(const float shift)
{
    return tuneTo(std::max(0.0f, std::min(DIAL_WIDTH, _arrow->getPositionX() + shift)));
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

    const float height = DIAL_HEIGHT - ARROW_RECT_HEIGHT;
    const float width = ARROW_THICKNESS * 2.0f;
    const float offset = ARROW_THICKNESS * 0.5f;
    const float length = height * 0.8f;
    const Vec2 points[7] {
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

float SignalDial::tuneTo(const float x) const
{
    _arrow->setPositionX(x);
    const float value = x / DIAL_WIDTH;
    _label->setString(StringUtils::format("TUNE %03d", static_cast<int>(value * 100)));
    return value;
}

void SignalDial::reset() const
{
    tuneTo(DIAL_WIDTH / 2.0f);
    _lockBar->setScaleX(0.0f);
}
