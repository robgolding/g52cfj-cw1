#pragma once
#include "DisplayableObject.h"
#include "MovementPosition.h"

class PacmanMain;

class PacmanObject : public DisplayableObject
{
    public:
        PacmanObject(PacmanMain* pEngine, int iMapX, int iMapY);
        ~PacmanObject(void);
        int GetXDiffForDirection(int iDir);
        int GetYDiffForDirection(int iDir);
        void MoveTo(int iMapx, int iMapY);

    protected:
        PacmanMain* m_pMainEngine;
        MovementPosition m_oMover;
        int m_iMapX;
        int m_iMapY;
        int m_iDir;
};
