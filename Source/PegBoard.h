#pragma once

#include "axmol.h"
#include "PegCell.h"

using PegCoords = std::pair<int, int>;

class PegBoard : public ax::DrawNode
{
public:
    static constexpr float SIZE = 460.0f;

    bool init() override;
    
    // implement the "static create()" method manually
    CREATE_FUNC(PegBoard);

    void reset() const;

    PegCoords onTouch(ax::Touch* touch) const;

    void setSelected(const PegCoords& coords, bool select) const
    { _cells[coords.first][coords.second]->setSelected(select); }

    void setPeg(const PegCoords& coords, const bool peg) const { _cells[coords.first][coords.second]->setPeg(peg); }
    bool hasPeg(const PegCoords& coords) const { return _cells[coords.first][coords.second]->hasPeg(); }

    bool hasJumps();

private:
    const ax::Color4F COLOR = ax::Color4F(ax::Color4B(0x2b, 0x33, 0x40, 0xff));
    const float OFFSET = 20.0f;
    const float RADIUS = 16.0f;
    const int LENGTH = 7;

    std::vector<std::vector<PegCell*>> _cells;

    void initCells();
    bool canJump(int col, int row) const;
};
