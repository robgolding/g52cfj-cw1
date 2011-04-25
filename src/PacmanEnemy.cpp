#include "PacmanMain.h"
#include "PacmanEnemy.h"
#include "PacmanAI.h"

PacmanEnemy::PacmanEnemy(PacmanMain* pEngine, int iMapX, int iMapY, PacmanAI* pAI)
: PacmanObject(pEngine, iMapX, iMapY)
, m_pAI(pAI)
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
    int iAIDir = m_pAI->GetMove(this, m_iMapX, m_iMapY);

    if (iAIDir != -1)
    {
        if (rand()%3 == 0 && CanMoveInDirection(iAIDir))
            m_iDir = iAIDir;
        MoveInDirection(m_iDir, iCurrentTime);
    }
    else
    {
        // Don't move!
        return;
    }
}

void PacmanEnemy::HandleMovementNotFinished(int iCurrentTime)
{
    PacmanObject::HandleMovementNotFinished(iCurrentTime);
}
