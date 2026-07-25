#pragma once

#include "axmol.h"
#include "RoundedRect.h"

class Disk : public RoundedRect
{
public:
    static constexpr float Height = 26.0f;

    virtual bool init(unsigned char size);
    
    static Disk* create(unsigned char size);

    unsigned char getSize() const { return _size; }

private:
    static const ax::Color4F Colors[5];

    unsigned char _size = 0;
};
