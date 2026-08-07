#pragma once

#include "2d/Menu.h"

class VNButton : public ax::Menu
{
public:
    static VNButton* create(const std::string& text, const ax::ccMenuCallback& callback);

    virtual bool init(const std::string& text, const ax::ccMenuCallback& callback);

private:
    static ax::Color4F BG_COLOR;
    static ax::Color4F BORDER_COLOR;
    static ax::Size PADDING;
    static constexpr float FONT_SIZE = 36.0f;
};
