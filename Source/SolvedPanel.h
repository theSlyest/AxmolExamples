#pragma once

#include "2d/DrawNode.h"
#include "2d/Label.h"
#include "2d/MenuItem.h"

class SolvedPanel : public ax::DrawNode
{
public:

    bool init() override;
    
    // implement the "static create()" method manually
    CREATE_FUNC(SolvedPanel);

    void setValues(const std::string& mirrors, const std::string& best) const;
    void setButtonCallback(const ax::ccMenuCallback &callback) { _btnCallback = callback; }

private:
    static constexpr float BORDER = 4.0f;
    static const ax::Color4F BG_COLOR;
    static const ax::Color4F BORDER_COLOR;
    static const ax::Size SIZE;

    ax::Label* _mirrorLbl = nullptr;
    ax::Label* _bestLbl = nullptr;

    ax::ccMenuCallback _btnCallback;

    void menuItemCallback(ax::Object* sender) const;
};
