#include "Peg.h"

USING_NS_AX;

bool Peg::init()
{
    if ( !RoundedRect::init(16.0f, 300.0f, 6.0f, Color4F(0.71f, 0.54f, 0.31f, 1.0f)) )
    {
        return false;
    }

    return true;
}

unsigned char Peg::getTopDisk() const {
    if (_disks.empty()) 
        return 0; // No disks on this peg
    
    return _disks.back();
}

void Peg::removeTopDisk() 
{ 
    if (_disks.empty())
        return; // No disks to remove

    _disks.pop_back();
}