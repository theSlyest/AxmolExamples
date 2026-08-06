#pragma once

#include "axmol.h"

class BalancerValue : public ax::DrawNode
{
public:
    static BalancerValue* create(const ax::Color4F& color, const std::string& name);

    virtual bool init(const ax::Color4F& color, const std::string& name);

    void setValue(const std::string& value) const { _label->setString(_name + ": " + value); }

private:
    static ax::Size PADDING;
    static ax::Color4F BG_COLOR;
    static constexpr float BORDER = 4.0f;

    ax::Label* _label = nullptr;
};
