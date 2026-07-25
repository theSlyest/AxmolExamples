#include "Disk.h"

USING_NS_AX;

const Color4F Disk::Colors[5] = {
    Color4F(0.31f, 0.62f, 0.91f, 1.0f),
    Color4F(0.37f, 0.77f, 0.43f, 1.0f),
    Color4F(0.91f, 0.78f, 0.23f, 1.0f),
    Color4F(0.91f, 0.57f, 0.24f, 1.0f),
    Color4F(0.89f, 0.36f, 0.36f, 1.0f)
};

bool Disk::init(unsigned char size)
{
    if (size == 0 || size > 5)
        return false;

    if ( !RoundedRect::init(40.0f + size * 30.0f, Disk::Height, 8.0f, Colors[size - 1]) )
        return false;

    _size = size;
    return true;
}

Disk *Disk::create(const unsigned char size)
{
    if (auto pRet = new (std::nothrow) Disk(); pRet && pRet->init(size))
    {
        pRet->autorelease();
        return pRet;
    }
    else
    {
        delete pRet;
        pRet = nullptr;
        return nullptr;
    }
}
