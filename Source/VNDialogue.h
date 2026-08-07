#pragma once

#include "2d/DrawNode.h"
#include "2d/MenuItem.h"

class VNDialogue : public ax::DrawNode
{
public:
    static constexpr float MARGIN = 20.0f;

    static VNDialogue* create(const std::string& title, const std::string& body, const ax::ccMenuCallback& callback);

    virtual bool init(const std::string& title, const std::string& body, const ax::ccMenuCallback& callback);

private:
    static ax::Color4F COLOR;
    static ax::Color4F SEP_COLOR;
    static ax::Vec2 PADDING;
    static ax::Size SIZE;
    static constexpr float HEIGHT = 220.0f;
    static constexpr float TITLE_FONT = 32.0f;
    static constexpr float BODY_FONT = 24.0f;
    static constexpr float BODY_LINE = 1.4f;
    static constexpr float RADIUS = 16.0f;

    void drawBackground();
};
