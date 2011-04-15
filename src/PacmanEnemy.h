#pragma once
#include "PacmanObject.h"

class PacmanEnemy : public PacmanObject
{
    public:
        PacmanEnemy(PacmanMain* pEngine, int iMapX, int iMapY);
        void Draw();
    protected:
        void HandleMovementFinished(int iCurrentTime);
        void HandleMovementNotFinished(int iCurrentTime);
};
