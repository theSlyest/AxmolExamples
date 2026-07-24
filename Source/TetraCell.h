#ifndef __TETRA_STACK_CELL_H__
#define __TETRA_STACK_CELL_H__

#include "axmol.h"

enum class Tetromino { I, O, T, S, Z, L, J }; // 'I' for the straight line, 'O' for the square,...

class TetraCell : public ax::DrawNode
{
public:
    static constexpr float SIZE = 24.0f;

    virtual bool init(Tetromino mino);
    
    // implement the "static create()" method manually
    static TetraCell* create(Tetromino mino);

    void setTetromino(Tetromino mino);

private:
    static const std::map<Tetromino, std::pair<ax::Color4F, ax::Color4F>> COLORS;
    static const ax::Color4F BORDER_COLOR;
};

#endif // __TETRA_STACK_CELL_H__
