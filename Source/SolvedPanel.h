#ifndef __LASER_MIRROR_SOLVED_H__
#define __LASER_MIRROR_SOLVED_H__

#include "axmol.h"

class SolvedPanel : public ax::DrawNode
{
public:

    virtual bool init();
    
    // implement the "static create()" method manually
    CREATE_FUNC(SolvedPanel);

    void setValues(const std::string& mirrors, const std::string& best);
    void setButtonCallback(const ax::ccMenuCallback &callback) { _btnCallback = callback; }

private:
    static constexpr float BORDER = 4.0f;
    static const ax::Color4F BG_COLOR;
    static const ax::Color4F BORDER_COLOR;
    static const ax::Size SIZE;

    ax::Label* _mirrorLbl;
    ax::Label* _bestLbl;

    ax::ccMenuCallback _btnCallback;

    void menuItemCallback(ax::Object* sender);
};

#endif // __LASER_MIRROR_VALUE_H__
