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
    if (!IsVisible())
        return;

    int colour;
    if (m_pMainEngine->IsInPowerupState())
    {
        if (m_pMainEngine->GetPowerupRemaining() < 2000)
            if (m_pMainEngine->GetModifiedTime() % 500 > 250)
                colour = 0xffffff;
            else
                colour = 0xff00ff;
        else
            colour = 0xff00ff;
    }
    else
        colour = 0x00ffff;

    m_pMainEngine->DrawScreenOval(
            m_iCurrentScreenX - m_iDrawWidth / 2,
            m_iCurrentScreenY - m_iDrawHeight / 2,
            m_iCurrentScreenX + m_iDrawWidth / 2,
            m_iCurrentScreenY + m_iDrawHeight / 2,
            colour);

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
