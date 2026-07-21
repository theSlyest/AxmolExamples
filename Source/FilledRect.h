#ifndef __BB_FILLEDRECT_H__
#define __BB_FILLEDRECT_H__

#include "axmol.h"

class FilledRect : public ax::DrawNode
{
protected:
    virtual bool init(const ax::Size& size, const ax::Color4F& color);
    
    static FilledRect* create(const ax::Size& size, const ax::Color4F& color);

    void setFillColor(const ax::Color4F& color);
};

#endif // __BB_FILLEDRECT_H__
