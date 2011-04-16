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
        void RedrawBackground();
        void GetRedrawRect(SDL_Rect *pRect);
        void StoreLastScreenPositionAndUpdateRect();
        int GetXDiffForDirection(int iDir);
        int GetYDiffForDirection(int iDir);
        int* GetBoundingBox();
        void MoveTo(int iMapx, int iMapY);
        virtual int GetXCentre();
        virtual int GetYCentre();
    protected:
        PacmanMain* m_pMainEngine;
        PacmanMover m_oMover;
        int m_iMapX;
        int m_iMapY;
        int m_iDir;
        int m_iXUpdateRect;
        int m_iYUpdateRect;
        int m_iWUpdateRect;
        int m_iHUpdateRect;
        virtual int GetScreenPosForMapX(int iMapX);
        virtual int GetScreenPosForMapY(int iMapY);
        virtual void HandleMovementFinished(int iCurrentTime);
        virtual void HandleMovementNotFinished(int iCurrentTime);
};
