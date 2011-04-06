#pragma once
#include "PacmanObject.h"
#include "PacmanEnemy.h"

class PacmanPlayer : public PacmanObject
{
    public:
        PacmanPlayer(PacmanMain* pEngine, int iMapX, int iMapY);
        void Draw();
        void DoUpdate(int iCurrentTime);
        void CollidedWith(PacmanEnemy* enemy);
    private:
        int m_iNextDir;
};
