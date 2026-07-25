#include "MineCell.h"

USING_NS_AX;

const std::vector<Color4B> MineCell::COUNT_COLORS {
    Color4B(48, 139, 243, 255),
    Color4B(95, 214, 63, 255),
    Color4B(232, 63, 48, 255),
    Color4B(16, 16, 184, 255),
    Color4B(184, 8, 8, 255),
    Color4B(24, 214, 214, 255),
    Color4B(0, 0, 0, 255),
    Color4B(128, 128, 128, 255),
    Color4B(214, 24, 214, 255)
};

const Color4F MineCell::BG_COLOR(0.75f, 0.75f, 0.75f, 1.0f);
const Color4F MineCell::BORDER_COLOR(0.0f, 0.0f, 0.0f, 1.0f);
const Color4F MineCell::COVER_COLOR(0.5f, 0.5f, 0.5f, 1.0f);
const Color4F MineCell::HOVER_COLOR(1.0f, 1.0f, 0.44f, 1.0f);
const Color4F MineCell::MINE_COLOR(0.0f, 0.0f, 0.0f, 1.0f);

bool MineCell::init()
{
    if (!DrawNode::init())
    {
        return false;
    }

    Size size(SIZE, SIZE);
    setContentSize(size);

    _mineCount = 0;

    initBackground();
    initLabel();
    initMine();
    initCover();
 
    return true;
}

void MineCell::initBackground()
{
    drawSolidRect(Vec2::ZERO, Vec2(_contentSize), BG_COLOR);
    float offset = BORDER / 2.0f;
    drawRect(Vec2(offset, offset), Vec2(SIZE - offset, SIZE - offset), BORDER_COLOR, BORDER);
}

void MineCell::initCover()
{
    float lineWidth = 2.0f;
    float offset = lineWidth / 2.0f;
    float start = BORDER + offset;
    float end = SIZE - BORDER - offset;

    _cover = DrawNode::create();
    _cover->setContentSize(_contentSize);
    _cover->drawSolidRect(Vec2(BORDER, BORDER), Vec2(end, end), COVER_COLOR, lineWidth);

    addChild(_cover, 1);

    initFlag();
    initHover();
    initCross();
}

void MineCell::initHover()
{
    float hOffset = HOVER / 2.0f;

    _hover = DrawNode::create();
    _hover->setVisible(false);
    _hover->drawRect(Vec2(hOffset, hOffset), Vec2(SIZE - hOffset, SIZE - hOffset), HOVER_COLOR, HOVER);

    _cover->addChild(_hover);
}

void MineCell::initMine()
{
    Vec2 center(SIZE / 2.0f, SIZE / 2.0f);
    float length = MINE_RADIUS_OUT * 2.0f;
    float offset = (SIZE - length) / 2.0f;

    _mine = DrawNode::create();
    _mine->setContentSize(_contentSize);
    _mine->setVisible(false);

    _mine->drawSolidCircle(center, MINE_RADIUS_IN, 0.0f, MINE_RADIUS_IN * M_PI, MINE_COLOR);
    _mine->drawLine(Vec2(offset, SIZE / 2.0f), Vec2(SIZE - offset, SIZE / 2.0f), MINE_COLOR, 4.0f);
    _mine->drawLine(Vec2(SIZE / 2.0f, offset), Vec2(SIZE / 2.0f, SIZE - offset), MINE_COLOR, 4.0f);

    addChild(_mine);
}

void MineCell::initFlag()
{
    Size size(36.0f, 36.0f);
    _flag = DrawNode::create();
    _flag->setContentSize(size);
    _flag->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    _flag->setPosition(Vec2(_contentSize / 2.0f));

    _flag->drawSolidRect(Vec2::ZERO, Vec2(28.0f, 6.0f), Color4F::BLACK, 4.0f);
    _flag->drawLine(Vec2(12.0f, 4.0f), Vec2(12.0f, 36.0f), Color4F::BLACK, 4.0f);

    Vec2 points[3] = {
        Vec2(13.0f, 14.0f),
        Vec2(13.0f, 34.0f),
        Vec2(36.0f, 24.0f)
    };
    _flag->drawSolidPoly(points, 3, Color4F::RED);

    _flag->setVisible(false);
    _cover->addChild(_flag);
}

void MineCell::initCross()
{
    float offset = 8.0f;
    _cross = DrawNode::create();
    _cross ->setContentSize(_contentSize);
    _cross->setVisible(false);

    _cross->drawLine(Vec2(offset, offset), Vec2(SIZE - offset, SIZE - offset), Color4F::RED, 6.0f);
    _cross->drawLine(Vec2(offset, SIZE - offset), Vec2(SIZE - offset, offset), Color4F::RED, 6.0f);

    _cover->addChild(_cross);
}

void MineCell::initLabel()
{
    _label = Label::createWithTTF(" ", "fonts/arial.ttf", 48.0f);
    _label->setPosition(SIZE / 2.0f, SIZE / 2.0f);
    addChild(_label);
}

void MineCell::setMineCount(int count)
{
    if (_mine->isVisible())
        return;

    if (count < 0 || count > 9)
        return;

    _mineCount = count;

    if (count == 0) {
        _label->setString("");
    }
    else if (count > 0 && count < 10) {
        _label->setString(std::to_string(count));
        _label->setTextColor(COUNT_COLORS[count - 1]);
    }
}

