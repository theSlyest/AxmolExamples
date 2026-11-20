#pragma once

#include "2d/DrawNode.h"
#include "2d/Label.h"

class HudValue : public ax::DrawNode
{
public:

    virtual bool init(const std::string& name, const ax::Color4F& color);
    
    // implement the "static create()" method manually
    static HudValue* create(const std::string& name, const ax::Color4F& color);

    void setValue(const std::string& value) const;

private:
    static constexpr float BORDER = 4.0f;
    static const ax::Color4F BG_COLOR;

    std::string _title;
    ax::Label* _label = nullptr;
};
