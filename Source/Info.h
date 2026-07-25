#pragma once

#include "axmol.h"

class Info : public ax::DrawNode
{
public:
    bool init() override;
    
    CREATE_FUNC(Info);

    void setPieceCount(int val, bool isBlack = false) const;
    void setTurn(bool isBlack = false) const;
    void setMoveCount(int val) const;
    void setInstruction(const std::string& instruction, bool isAlert = false) const;
    void setResult(int result) const;

private:
    static const ax::Color4F NormalColor;
    static const ax::Color4F AlertColor;

    ax::Label* _turnLabel = nullptr;
    ax::Label* _instructionLabel = nullptr;
    ax::Label* _moveCountLabel = nullptr;
    ax::Label* _blackCountLabel = nullptr;
    ax::Label* _whiteCountLabel = nullptr;
    DrawNode* _turnNode = nullptr;

    void initTurnNode();
};
