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
    protected:
        void DetectCollision(int iCurrentTime);
        void HandleMovementFinished(int iCurrentTime);
        void HandleMovementNotFinished(int iCurrentTime);
        virtual void RequestNewDirection(int iDirection, int iCurrentTime);
    private:
        int m_iNextDir;
};
