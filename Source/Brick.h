#ifndef __BB_BRICK_H__
#define __BB_BRICK_H__

#include "FilledRect.h"
#include "axmol.h"

class Brick : public FilledRect
{
public:
    static const ax::Size BrickSize;

    static Brick* create(bool isStrong = false);

    virtual bool init(bool isStrong);    

    bool isStrong() const { return _isStrong; }

    bool hit();

private:
    static const ax::Color4F StrongColor;
    static const ax::Color4F WeakColor;
    static const ax::Color4F NormalColor;

    bool _isStrong;
    bool _isOk;
};

#endif // __BB_BRICK_H__
