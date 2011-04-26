#include "header.h"

#include "DisplayableObject.h"
#include "PacmanObject.h"
#include "PacmanMain.h"

PacmanObject::PacmanObject(PacmanMain* pEngine, int iMapX, int iMapY)
: DisplayableObject(pEngine)
, m_pMainEngine(pEngine)
, m_iOriginalMapX(iMapX)
, m_iOriginalMapY(iMapY)
, m_iMapX(iMapX)
, m_iMapY(iMapY)
, m_iDir(0)
{
    m_iDrawWidth = m_pMainEngine->GetTileManager().GetTileWidth();
    m_iDrawHeight = m_pMainEngine->GetTileManager().GetTileHeight();

    m_iPreviousScreenX = m_iCurrentScreenX = GetScreenPosForMapX(iMapX);
    m_iPreviousScreenY = m_iCurrentScreenY = GetScreenPosForMapY(iMapY);

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

void PacmanObject::RedrawBackground()
{
    m_pEngine->CopyBackgroundPixels(m_iPreviousScreenX - m_iDrawWidth / 2 - 1,
        m_iPreviousScreenY - m_iDrawHeight / 2 - 1, m_iDrawWidth + 2, m_iDrawHeight + 2);
}

void PacmanObject::GetRedrawRect(SDL_Rect *pRect)
{
    pRect->x = m_iXUpdateRect;
    pRect->y = m_iYUpdateRect;
    pRect->w = m_iWUpdateRect;
    pRect->h = m_iHUpdateRect;
}

void PacmanObject::StoreLastScreenPositionAndUpdateRect()
{
    // Our initial rectangle that needs updating is the current screen position
    //int x1 = m_iCurrentScreenX - m_iDrawWidth / 2 - 1;
    //int x2 = m_iCurrentScreenX + m_iDrawWidth / 2 + 1;
    //int y1 = m_iCurrentScreenY - m_iDrawHeight / 2 - 1;
    //int y2 = m_iCurrentScreenY + m_iDrawHeight / 2 + 1;

    int* box = GetBoundingBox();
    int x1 = box[0];
    int y1 = box[1];
    int x2 = box[2];
    int y2 = box[3];

    static int i = 0;
    // The screen area changed is the previous position + the new position
    // so grow the rectangle according to the previous position of the object
    if (m_iPreviousScreenX < m_iCurrentScreenX)
        x1 = m_iPreviousScreenX - m_iDrawWidth / 2 - 1;
    if (m_iPreviousScreenX > m_iCurrentScreenX)
        x2 = m_iPreviousScreenX + m_iDrawWidth / 2 + 1;
    if (m_iPreviousScreenY < m_iCurrentScreenY)
        y1 = m_iPreviousScreenY - m_iDrawHeight / 2 - 1;
    if (m_iPreviousScreenY > m_iCurrentScreenY)
        y2 = m_iPreviousScreenY + m_iDrawHeight / 2 + 1;

    // Now set up the rectangle
    m_iXUpdateRect = x1;
    m_iWUpdateRect = x2 - x1;
    m_iYUpdateRect = y1;
    m_iHUpdateRect = y2 - y1;

    // Store the location that was last drawn at, so that it can be removed again
    m_iPreviousScreenX = m_iCurrentScreenX;
    m_iPreviousScreenY = m_iCurrentScreenY;
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

int* PacmanObject::GetBoundingBox()
{
    int x1 = m_iCurrentScreenX - m_iDrawWidth / 2 - 1;
    int x2 = m_iCurrentScreenX + m_iDrawWidth / 2 + 1;
    int y1 = m_iCurrentScreenY - m_iDrawHeight / 2 - 1;
    int y2 = m_iCurrentScreenY + m_iDrawHeight / 2 + 1;

    int* box = new int[4];

    box[0] = x1;
    box[1] = y1;
    box[2] = x2;
    box[3] = y2;

    return box;
}

void PacmanObject::MoveTo(int iMapX, int iMapY)
{
    m_iMapX = iMapX;
    m_iMapY = iMapY;
    m_iPreviousScreenX = m_iCurrentScreenX = GetScreenPosForMapX(iMapX);
    m_iPreviousScreenY = m_iCurrentScreenY = GetScreenPosForMapY(iMapY);
    m_oMover.Setup(0, 0, 0, 0, 0, 0);
}

void PacmanObject::MoveInDirection(int iDir, int iCurrentTime)
{
    if (CanMoveInDirection(iDir))
    {
        m_iMapX += GetXDiffForDirection(m_iDir);
        m_iMapY += GetYDiffForDirection(m_iDir);
        m_oMover.Setup(
                m_iCurrentScreenX,
                m_iCurrentScreenY,
                GetScreenPosForMapX(m_iMapX),
                GetScreenPosForMapY(m_iMapY),
                iCurrentTime,
                iCurrentTime+400);
    }
}

int PacmanObject::CanMoveInDirection(int iDir)
{
    if (m_pMainEngine->GetTileManager().GetValue(
                m_iMapX + GetXDiffForDirection(iDir),
                m_iMapY + GetYDiffForDirection(iDir)) != 1)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

int PacmanObject::GetXCentre()
{
    return m_iCurrentScreenX;
}

int PacmanObject::GetYCentre()
{
    return m_iCurrentScreenY;
}

void PacmanObject::ResetPosition()
{
    MoveTo(m_iOriginalMapX, m_iOriginalMapY);
}

int PacmanObject::GetScreenPosForMapX(int iMapX)
{
    int iTileWidth = m_pMainEngine->GetTileManager().GetTileWidth();
    int iBaseScreenX = m_pMainEngine->GetTileManager().GetBaseScreenX();
    return iMapX * iTileWidth + iBaseScreenX + m_iDrawWidth / 2;
}

int PacmanObject::GetScreenPosForMapY(int iMapY)
{
    int iTileHeight = m_pMainEngine->GetTileManager().GetTileHeight();
    int iBaseScreenY = m_pMainEngine->GetTileManager().GetBaseScreenY();
    return iMapY * iTileHeight + iBaseScreenY + m_iDrawHeight / 2;
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
