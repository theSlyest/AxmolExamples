#pragma once

#include <map>
#include <vector>

#include "2d/Node.h"
#include "BomberBomb.h"
#include "GridModel.h"
#include "GridTile.h"

// Draws a GridModel: one GridTile per cell, plus a bomb node on each tile that
// holds a bomb. Call refresh() when the model reports a tile change.
class BomberGrid : public ax::Node
{
public:
    // The model must outlive the view.
    static BomberGrid* create(const GridModel& model);

    bool init(const GridModel& model);

    void refresh(int col, int row);

private:
    const GridModel* _model = nullptr;
    std::vector<std::vector<GridTile*>> _tiles;
    std::map<GridCoords, BomberBomb*> _bombs;
};
