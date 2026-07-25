#ifndef __MINES_CELL_H__
#define __MINES_CELL_H__

#include "axmol.h"

class MineCell : public ax::DrawNode
{
public:
    static constexpr float SIZE = 60.0f;

    virtual bool init();
    
    // implement the "static create()" method manually
    CREATE_FUNC(MineCell);

    void setCovered(bool isCovered) { _cover->setVisible(isCovered); }
    bool isCovered() const { return _cover->isVisible(); }

    void setMine(bool isMine) { _mine->setVisible(isMine); }
    bool isMine() const { return _mine->isVisible(); }

    void setMineCount(int count);
    int getMineCount() const { return _mineCount; };

    void setHovered(bool isHovered) { _hover->setVisible(isHovered); }
    
    void setFlagged(bool isFlagged) { _flag->setVisible(isFlagged); }

    void setCrossed(bool isCrossed) { _cross->setVisible(isCrossed); }

    bool isFlagged() const { return _cover->isVisible() && _flag->isVisible(); }

private:
    static constexpr float BORDER = 2.0f;
    static constexpr float HOVER = 2.0f;
    static constexpr float MINE_RADIUS_IN = 16.0f;
    static constexpr float MINE_RADIUS_OUT = 20.0f;

    static const std::vector<ax::Color4B> COUNT_COLORS;
    static const ax::Color4F BG_COLOR;
    static const ax::Color4F BORDER_COLOR;
    static const ax::Color4F HOVER_COLOR;
    static const ax::Color4F COVER_COLOR;
    static const ax::Color4F COVER_LIGHT;
    static const ax::Color4F COVER_DARK;
    static const ax::Color4F MINE_COLOR;

    int _mineCount;

    ax::Label* _label;
    ax::DrawNode* _cover;
    ax::DrawNode* _hover;
    ax::DrawNode* _mine;
    ax::DrawNode* _flag;
    ax::DrawNode* _cross;

    void initBackground();
    void initCover();
    void initHover();
    void initMine();
    void initFlag();
    void initCross();
    void initLabel();
};

#endif // __MINES_CELL_H__
