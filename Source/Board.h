#pragma once

#include "axmol.h"
#include "BoardSquare.h"

class Board : public ax::DrawNode
{
public:
    bool init() override;
    
    CREATE_FUNC(Board);

    bool isLegal(const int row, const int col) const { return _squares[row][col]->isLegal(); }
    void setLegal(const int row, const int col, const bool isLegal) const { _squares[row][col]->setLegal(isLegal); }

    void setHovered(const int row, const int col, const bool hovered) const { _squares[row][col]->setHovered(hovered); }

private:
    std::vector<std::vector<BoardSquare*>> _squares;
};
