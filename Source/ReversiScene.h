#pragma once

#include "axmol.h"
#include "Board.h"
#include "Info.h"
#include "Piece.h"

class ReversiScene : public ax::Scene
{
public:
    virtual bool init();
    
    // implement the "static create()" method manually
    CREATE_FUNC(ReversiScene);

private:
    std::vector<std::vector<Piece*>> _pieces;
    Info* _info;
    Board* _board;
    bool _prevLegal;

    bool _isBlackTurn;
    int _moveCount;
    std::vector<int> _pieceCounts;
    std::pair<int, int> _hovered;

    ax::EventListenerMouse* _mouseListener;

    void gameStart();
    void nextTurn();
    void gameOver() const;

    void addPiece(int row, int col, bool isBlack = false);

    bool onMouseMove(const ax::EventMouse* event);
    bool onMouseUp(ax::EventMouse* event);

    std::pair<int, int> squareFromLocation(const ax::Vec2& location);

    int checkLegal();
    bool keepChecking(int row, int col, bool isFirst) const;
    void switchColorFrom(int row, int col);
    void switchPieceColor(int row, int col);
};
