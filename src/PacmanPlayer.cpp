#include "header.h"

#include "PacmanObject.h"
#include "PacmanMain.h"
#include "PacmanPlayer.h"
#include "PacmanEnemy.h"

PacmanPlayer::PacmanPlayer(PacmanMain* pEngine, int iMapX, int iMapY)
: PacmanObject(pEngine, iMapX, iMapY)
, m_iNextDir(0)
{
}

void PacmanPlayer::Draw()
{
    if ( !IsVisible() )
        return;

    m_pMainEngine->DrawScreenOval(
            m_iCurrentScreenX - m_iDrawWidth / 2,
            m_iCurrentScreenY - m_iDrawHeight / 2,
            m_iCurrentScreenX + m_iDrawWidth / 2,
            m_iCurrentScreenY + m_iDrawHeight / 2,
            0xffff00);

    StoreLastScreenPositionAndUpdateRect();
}

void PacmanPlayer::DoUpdate(int iCurrentTime)
{
    int iNextDir = -1;
    if (m_pMainEngine->IsKeyPressed(SDLK_UP))
        iNextDir = 0;
    if (m_pMainEngine->IsKeyPressed(SDLK_RIGHT))
        iNextDir = 1;
    if (m_pMainEngine->IsKeyPressed(SDLK_DOWN))
        iNextDir = 2;
    if (m_pMainEngine->IsKeyPressed(SDLK_LEFT))
        iNextDir = 3;

    if (iNextDir >= 0)
        RequestNewDirection(iNextDir, iCurrentTime);

    PacmanObject::DoUpdate(iCurrentTime);
    DetectCollision(iCurrentTime);
}

void PacmanPlayer::CollidedWith(PacmanEnemy* enemy)
{
}

void PacmanPlayer::DetectCollision(int iCurrentTime)
{
    DisplayableObject* pObject;

    int myRadius = (GetBoundingBox()[2] - GetBoundingBox()[0]) / 2;

    for (int iObjectId = 0;
            (pObject = m_pMainEngine->GetDisplayableObject(iObjectId)) != NULL;
            iObjectId++)
    {
        if (pObject == this)
            continue;

        PacmanObject* pPacmanObject = dynamic_cast<PacmanObject*>(pObject);
        if (pPacmanObject != NULL && pPacmanObject->IsVisible())
        {
            int otherRadius = (pPacmanObject->GetBoundingBox()[2] -
                    pPacmanObject->GetBoundingBox()[0]) / 2;
            int iXDiff = pPacmanObject->GetXCentre() - m_iCurrentScreenX;
            int iYDiff = pPacmanObject->GetYCentre() - m_iCurrentScreenY;

            if ( ((iXDiff*iXDiff)+(iYDiff*iYDiff))
                < ((myRadius + otherRadius)*(myRadius + otherRadius)))
            {
                PacmanEnemy* enemy = dynamic_cast<PacmanEnemy*>(pPacmanObject);
                if (enemy != NULL && enemy->IsVisible())
                    m_pMainEngine->CollisionDetected(this, enemy);
                return;
            }
        }
    }
}

void PacmanPlayer::HandleMovementFinished(int iCurrentTime)
{
    PacmanTileManager& tm = m_pMainEngine->GetTileManager();

    switch (tm.GetValue(m_iMapX, m_iMapY))
    {
        case 2:
        case 3:
        case 4:
        case 5:
        case 6:
        case 7:
            tm.UpdateTile(m_pMainEngine, m_iMapX, m_iMapY, 0);
            m_pMainEngine->AtePellet();
            break;
        case 8:
            tm.UpdateTile(m_pMainEngine, m_iMapX, m_iMapY, 0);
            break;
        case 9:
            MoveTo(abs(m_iMapX - m_pMainEngine->GetTileManager().GetMapWidth()) - 1, m_iMapY);
            break;
    }

    // Are we allowed to move onto the tile requested?
    if (tm.GetValue(m_iMapX + GetXDiffForDirection(m_iNextDir),
                m_iMapY + GetYDiffForDirection(m_iNextDir)) != 1)
    {
        m_iDir = m_iNextDir;
    }

    // Are we allowed to move onto the tile?
    if (tm.GetValue(m_iMapX + GetXDiffForDirection(m_iDir),
                m_iMapY + GetYDiffForDirection(m_iDir)) != 1)
    {
        // Allow move - set up new movement now
        m_iMapX += GetXDiffForDirection(m_iDir);
        m_iMapY += GetYDiffForDirection(m_iDir);

        m_oMover.Setup(
                m_iCurrentScreenX,
                m_iCurrentScreenY,
                GetScreenPosForMapX(m_iMapX),
                GetScreenPosForMapY(m_iMapY),
                iCurrentTime,
                iCurrentTime + 300);
    }
}

void PacmanPlayer::HandleMovementNotFinished(int iCurrentTime)
{
    PacmanObject::HandleMovementNotFinished(iCurrentTime);
}

void PacmanPlayer::RequestNewDirection(int iDirection, int iCurrentTime)
{
    if (m_oMover.HasMovementFinished(iCurrentTime))
    {
        m_iNextDir = iDirection;
        return;
    }

    if (iDirection != m_iNextDir)
    {
        if (abs(iDirection - m_iDir) == 2)
        {
            // go in the opposite direction
            m_iDir = m_iNextDir = iDirection;

            m_iMapX += GetXDiffForDirection(m_iDir);
            m_iMapY += GetYDiffForDirection(m_iDir);

            m_oMover.Reverse(iCurrentTime);
        }
        else
        {
            // request a "normal" change of direction
            m_iNextDir = iDirection;
        }
    }
}
