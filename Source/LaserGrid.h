#pragma once

#include <optional>
#include <utility>
#include <vector>

#include "2d/DrawNode.h"
#include "Emitter.h"
#include "GridCell.h"
#include "LaserBoard.h"
#include "Mirror.h"

namespace ax { class Touch; }

// Draws a LaserBoard: a frame, a cell for every position, the emitter, the
// mirrors and the beam. Call refresh() whenever the board has changed.
class LaserGrid : public ax::DrawNode
{
public:
    static constexpr int ROWS = LaserBoard::ROWS;
    static constexpr int COLS = LaserBoard::COLS;
    static const ax::Color4F BORDER_COLOR;

    // The board must outlive the view.
    static LaserGrid* create(const LaserBoard& board);

    bool init(const LaserBoard& board);

    // Redraw the mirrors and the beam from the board.
    void refresh() const;

    // The cell under a touch as (row, col), if the touch is on the grid.
    [[nodiscard]] std::optional<std::pair<int, int>> cellAt(ax::Touch* touch) const;

private:
    static constexpr float BORDER = 4.0f;

    const LaserBoard* _board = nullptr;

    std::vector<std::vector<GridCell*>> _cells;
    std::vector<std::vector<Mirror*>> _mirrors; // null where the cell is a wall or the target

    Emitter* _emitter = nullptr;

    void initCells();
    void initEmitter();
};
