#ifndef __SUDOKU_CELL_H__
#define __SUDOKU_CELL_H__

#include "axmol.h"

class GridCell : public ax::LayerColor
{
public:
    static constexpr float SIZE = 60.0f;

    virtual bool init(bool isDeeper);
    
    static GridCell* create(bool isDeeper = false);

    void select();
    void deselect();

    void setClueDigit(const char digit);
    void setDigit(const char digit);
    void clear();
    char getDigit() const { return _digit; }

    bool isClue() const { return _isClue; }

    void setConflict(bool hasConflict);

private:
    static const ax::Color4B BgColorLight;
    static const ax::Color4B BgColorDeep;
    static const ax::Color4B BgColorSelect;

    static const ax::Color4B DigitColorClue;
    static const ax::Color4B DigitColorUser;
    static const ax::Color4B DigitColorConflict;

    bool _isDeeper;
    bool _isClue;
    bool _isConflict;
    char _digit;

    ax::Label* _label;

    void setLabelDigit(const char digit);
};

#endif // __SUDOKU_CELL_H__
