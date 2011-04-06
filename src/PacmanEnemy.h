#pragma once
#include "PacmanObject.h"

class PacmanEnemy : public PacmanObject
{
    public:
        PacmanEnemy(PacmanMain* pEngine, int iMapX, int iMapY);
        void Draw();
        void DoUpdate(int iCurrentTime);
};
