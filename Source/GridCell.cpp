#include "GridCell.h"

USING_NS_AX;

const Color4B GridCell::BgColorLight(233, 233, 196, 255);
const Color4B GridCell::BgColorDeep(220, 205, 171, 255);
const Color4B GridCell::BgColorSelect(242, 214, 117, 255);

const Color4B GridCell::DigitColorClue(35, 38, 46, 255);
const Color4B GridCell::DigitColorUser(47, 109, 208, 255);
const Color4B GridCell::DigitColorConflict(209,56, 44, 255);

GridCell* GridCell::create(bool isDeeper)
{
    GridCell *pRet = new(std::nothrow) GridCell();
    if (pRet && pRet->init(isDeeper))
    {
        pRet->autorelease();
        return pRet;
    }
    else
    {
        delete pRet;
        pRet = nullptr;
        return nullptr;
    }
}

bool GridCell::init(bool isDeeper)
{
    if (!LayerColor::initWithColor(isDeeper ? BgColorDeep : BgColorLight, SIZE, SIZE))
    {
        return false;
    }

    _digit = 0;
    _isDeeper = isDeeper;
    _isClue = false;

    _label = Label::createWithTTF("", "fonts/arial.ttf", 40.0f);
    _label->setTextColor(DigitColorUser);
    _label->setPosition(SIZE / 2.0f, SIZE / 2.0f);
    addChild(_label);

    setContentSize(Size(SIZE, SIZE));

    return true;
}

void GridCell::select()
{
    setColor(Color3B(BgColorSelect));
}

void GridCell::deselect()
{
    setColor(Color3B(_isDeeper ? BgColorDeep : BgColorLight));
}

void GridCell::setClueDigit(const char digit)
{
    _isClue = true;
    _digit = digit;
    _label->setTextColor(DigitColorClue);
    setLabelDigit(digit);
}

void GridCell::setDigit(const char digit)
{
    if (_isClue)
        return;

    _digit = digit;
    _label->setTextColor(DigitColorUser);
    setLabelDigit(digit);
}

void GridCell::clear()
{
    _digit = 0;
    _label->setTextColor(DigitColorUser);
    _label->setString("");
}

void GridCell::setConflict(bool hasConflict)
{
    if (_isClue)
        return;

    _label->setTextColor(hasConflict ? DigitColorConflict : DigitColorUser);
}

void GridCell::setLabelDigit(const char digit) { _label->setString(std::string(1, digit)); }
