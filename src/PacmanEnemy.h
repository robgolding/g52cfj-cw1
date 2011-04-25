#pragma once
#include "PacmanObject.h"

class PacmanAI;

class PacmanEnemy : public PacmanObject
{
    public:
        PacmanEnemy(PacmanMain* pEngine, int iMapX, int iMapY, PacmanAI* pAI);
        void Draw();
    protected:
        void HandleMovementFinished(int iCurrentTime);
        void HandleMovementNotFinished(int iCurrentTime);
    private:
        PacmanAI* m_pAI;
};
