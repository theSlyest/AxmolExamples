#pragma once

#include "2d/Node.h"
#include "Touch.h"
#include "PentagoBlock.h"

using LinedSockets = std::map<bool, std::set<PentagoSocket*>>;

class PentagoBoard : public ax::Node
{
public:
    bool init() override;
    
    // implement the "static create()" method manually
    CREATE_FUNC(PentagoBoard);

    void reset();

    bool onTouch(ax::Touch* touch, bool isBlack) const;

    bool checkLines();

    LinedSockets getLines() const { return _lines; }
    void ringLines() const;

private:
    static constexpr float SPACING = 16.0f;

    std::vector<std::vector<PentagoBlock*>> _blocks;
    LinedSockets _lines;

    PentagoSocket* get(int col, int row) const;

    bool checkHorizontal();
    bool checkVertical();
    bool checkDiagonal();
    bool checkDiagonalOf5(int startRow);
    bool checkDiagonalOf6(bool lastRow);
};
