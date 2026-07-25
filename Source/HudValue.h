#ifndef __LASER_MIRROR_VALUE_H__
#define __LASER_MIRROR_VALUE_H__

#include "axmol.h"

class HudValue : public ax::DrawNode
{
public:

    virtual bool init(const std::string& name, const ax::Color4F& color);
    
    // implement the "static create()" method manually
    static HudValue* create(const std::string& name, const ax::Color4F& color);

    void setValue(const std::string& value);

private:
    static constexpr float BORDER = 4.0f;
    static const ax::Color4F BG_COLOR;

    ax::Label* _label;
};

#endif // __LASER_MIRROR_VALUE_H__
