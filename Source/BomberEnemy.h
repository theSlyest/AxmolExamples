#ifndef __BOMBERMAN_ENEMY_H__
#define __BOMBERMAN_ENEMY_H__

#include "axmol.h"

class BomberEnemy : public ax::DrawNode
{
public:
    static const float SIZE;
    static const float SPEED;

    virtual bool init();
    
    // implement the "static create()" method manually
    CREATE_FUNC(BomberEnemy);

private:
    static const ax::Color4F COLOR;

};

#endif // __BOMBERMAN_ENEMY_H__
