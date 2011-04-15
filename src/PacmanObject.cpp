#include "header.h"

#include "DisplayableObject.h"
#include "PacmanObject.h"
#include "PacmanMain.h"

PacmanObject::PacmanObject(PacmanMain* pEngine, int iMapX, int iMapY)
: DisplayableObject(pEngine)
, m_pMainEngine(pEngine)
, m_iMapX(iMapX)
, m_iMapY(iMapY)
, m_iDir(0)
{
    // The ball coordinate will be the centre of the ball
    // Because we start drawing half the size to the top-left.
    m_iStartDrawPosX = -25;
    m_iStartDrawPosY = -25;

    // Record the ball size as both height and width
    m_iDrawWidth = 50;
    m_iDrawHeight = 50;

    // Out item at specific coordinates
    m_iPreviousScreenX = m_iCurrentScreenX = iMapX*50+25+25;
    m_iPreviousScreenY = m_iCurrentScreenY = iMapY*50+25+40;

    // And make it visible
    SetVisible(true);
}

PacmanObject::~PacmanObject(void)
{
}

void PacmanObject::DoUpdate(int iCurrentTime)
{
    m_iPreviousScreenX = m_iCurrentScreenX;
    m_iPreviousScreenY = m_iCurrentScreenY;

    if (m_oMover.HasMovementFinished(iCurrentTime))
        HandleMovementFinished(iCurrentTime);
    else
        HandleMovementNotFinished(iCurrentTime);

    RedrawObjects();
}

int PacmanObject::GetXDiffForDirection(int iDir)
{
    switch(iDir % 4)
    {
        case 0: return 0; // Up
        case 1: return 1; // Right
        case 2: return 0; // Down
        case 3: return -1; // Left
        default: return 0; // Can never happen
    }
}

int PacmanObject::GetYDiffForDirection(int iDir)
{
    switch(iDir % 4)
    {
        case 0: return -1; // Up
        case 1: return 0; // Right
        case 2: return 1; // Down
        case 3: return 0; // Left
        default: return 0; // Can never happen
    }
}

void PacmanObject::MoveTo(int iMapX, int iMapY)
{
    m_iMapX = iMapX;
    m_iMapY = iMapY;
}

void PacmanObject::HandleMovementFinished(int iCurrentTime)
{
}

void PacmanObject::HandleMovementNotFinished(int iCurrentTime)
{
    m_oMover.Calculate(iCurrentTime);
    m_iCurrentScreenX = m_oMover.GetX();
    m_iCurrentScreenY = m_oMover.GetY();
}
