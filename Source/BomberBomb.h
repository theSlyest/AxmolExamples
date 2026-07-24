#ifndef __BOMBERMAN_BOMB_H__
#define __BOMBERMAN_BOMB_H__

#include "axmol.h"
#include "BomberPlayer.h"

class BomberBomb : public ax::DrawNode
{
public:
    static const float SIZE;

    virtual bool init(BomberPlayer* player);
    
    // implement the "static create()" method manually
    static BomberBomb* create(BomberPlayer* player);

    void releaseBomber();

private:
    static const ax::Color4F COLOR;

    BomberPlayer* _player;
};

#endif // __BOMBERMAN_BOMB_H__
