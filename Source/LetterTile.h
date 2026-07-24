#ifndef __LETTER_TILE_H__
#define __LETTER_TILE_H__

#include "axmol.h"

enum class TileState { DEFAULT, YELLOW, GREEN };

class LetterTile : public ax::Node
{
public:
    static constexpr float SIZE = 96.0f;

    virtual bool init();
    
    CREATE_FUNC(LetterTile);

    void erase();
    void setLetter(char letter);
    char getLetter() const { return _letter; }

    void setState(TileState state);
    TileState getState() const { return _state; }

private:
    static constexpr float LabelFontSize = 80.0f;
    static constexpr float BorderWidth = 2.0f;
    static const ax::Color4F Yellow;
    static const ax::Color4F Green;

    char _letter;
    TileState _state;
    ax::DrawNode* _background;
    ax::Label* _label;
};

#endif // __LETTER_TILE_H__
