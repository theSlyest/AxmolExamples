#ifndef __TETRA_STACK_GRID_H__
#define __TETRA_STACK_GRID_H__

#include "axmol.h"
#include "TetraCell.h"

using TetraShape = std::vector<std::vector<bool>>;
using GridPos = std::pair<int, int>;

struct LinesData {
    int number;
    LinesData(int nb) : number(nb) {}
};

class TetraGrid : public ax::DrawNode
{
public:
    static const std::map<Tetromino, TetraShape> SHAPES;
    static const std::string LOCK_EVENT;
    static const ax::Color4F COLOR;
    
    static constexpr float BORDER = 3.0f;
    static constexpr float LINE = 1.0f;

    virtual bool init();
    
    // implement the "static create()" method manually
    CREATE_FUNC(TetraGrid);

    bool pushBlock(Tetromino mino);

    virtual void update(float dt) override;

    void shiftBlock(bool right);
    void speedUpBlock(bool up);
    void turnBlock();
    void dropBlock();

    void reset();

private:
    static constexpr int COLS = 10;
    static constexpr int ROWS = 20;
    static constexpr float PERIOD = 2.0f / 3.0f;

    bool _hasBlock;

    float _elapsed;
    float _period;

    Tetromino _blockMino;
    GridPos _blockAxis;
    std::vector<GridPos> _blockState;
    TetraShape _blockShape;
    std::vector<TetraCell*> _blockCells;
    std::vector<std::vector<TetraCell*>> _cells;

    ax::Node* _cellNode;

    void drawGrid();
    void initCellNode();

    bool stepBlock();

    bool turnI(std::vector<GridPos>& state);
    bool turnDefault(std::vector<GridPos>& state);

    void lockBlock();
    int clearLines();
};

#endif // __TETRA_STACK_GRID_H__
