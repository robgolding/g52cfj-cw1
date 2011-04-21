#include "header.h"

#include "PacmanMain.h"
#include "PacmanEnemy.h"

PacmanEnemy::PacmanEnemy(PacmanMain* pEngine, int iMapX, int iMapY)
: PacmanObject(pEngine, iMapX, iMapY)
{
}

void PacmanEnemy::Draw()
{
    if ( !IsVisible() )
        return;

    m_pMainEngine->DrawScreenOval(
            m_iCurrentScreenX - m_iDrawWidth / 2,
            m_iCurrentScreenY - m_iDrawHeight / 2,
            m_iCurrentScreenX + m_iDrawWidth / 2,
            m_iCurrentScreenY + m_iDrawHeight / 2,
            0x00ffff);

    StoreLastScreenPositionAndUpdateRect();
}

void PacmanEnemy::HandleMovementFinished(int iCurrentTime)
{
    PacmanTileManager& tm = m_pMainEngine->GetTileManager();

    int iXDiff = m_pMainEngine->GetPlayer().GetXCentre() - GetXCentre();
    int iYDiff = m_pMainEngine->GetPlayer().GetYCentre() - GetYCentre();

    int iNewDir;

    if (abs(iXDiff) > abs(iYDiff))
    {
        if (iXDiff > 0)
            iNewDir = 1;
        else
            iNewDir = 3;
    }
    else
    {
        if (iYDiff > 0)
            iNewDir = 2;
        else
            iNewDir = 0;
    }

    if (rand() % 5 == 0)
        m_iDir = iNewDir;

    switch (tm.GetValue(
                m_iMapX + GetXDiffForDirection(m_iDir),
                m_iMapY + GetYDiffForDirection(m_iDir))
           )
    {
        case 0: // Passageway
        case 2: // Pellet
        case 3: // Pellet
        case 4: // Pellet
        case 5: // Pellet
        case 6: // Pellet
        case 7: // Pellet
        case 8: // Pellet
            // Allow move - set up new movement now
            m_iMapX += GetXDiffForDirection(m_iDir);
            m_iMapY += GetYDiffForDirection(m_iDir);
            m_oMover.Setup(
                    m_iCurrentScreenX,
                    m_iCurrentScreenY,
                    GetScreenPosForMapX(m_iMapX),
                    GetScreenPosForMapY(m_iMapY),
                    iCurrentTime,
                    iCurrentTime+400);
            break;
        case 1: // Wall
            m_iDir = rand()%4; // Rotate randomly
            break;
    }
}

void PacmanEnemy::HandleMovementNotFinished(int iCurrentTime)
{
    PacmanObject::HandleMovementNotFinished(iCurrentTime);
}
