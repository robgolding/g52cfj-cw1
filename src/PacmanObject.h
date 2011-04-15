#pragma once
#include "DisplayableObject.h"
#include "PacmanMover.h"

class PacmanMain;

class PacmanObject : public DisplayableObject
{
    public:
        PacmanObject(PacmanMain* pEngine, int iMapX, int iMapY);
        ~PacmanObject(void);
        void DoUpdate(int iCurrentTime);
        int GetXDiffForDirection(int iDir);
        int GetYDiffForDirection(int iDir);
        void MoveTo(int iMapx, int iMapY);
    protected:
        PacmanMain* m_pMainEngine;
        PacmanMover m_oMover;
        int m_iMapX;
        int m_iMapY;
        int m_iDir;
        virtual void HandleMovementFinished(int iCurrentTime);
        virtual void HandleMovementNotFinished(int iCurrentTime);
};
