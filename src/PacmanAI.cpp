#include "PacmanAI.h"
#include "PacmanMain.h"
#include "PacmanObject.h"

PacmanAI::PacmanAI(PacmanMain* pEngine)
: m_pMainEngine(pEngine)
{
}

int PacmanAI::GetMove(PacmanObject* pObj, int iMapX, int iMapY)
{
    PacmanTileManager& tm = m_pMainEngine->GetTileManager();

    int iXDiff = m_pMainEngine->GetPlayer().GetXCentre() - pObj->GetXCentre();
    int iYDiff = m_pMainEngine->GetPlayer().GetYCentre() - pObj->GetYCentre();

    int iNewDir;
    int piDirPrefs[2];

    if (iXDiff == 0)
        iXDiff = 1;
    if (iYDiff == 0)
        iYDiff = 1;

    if (abs(iXDiff) > abs(iYDiff) && rand()%2 == 0)
    {
        piDirPrefs = {(iXDiff/abs(iXDiff))*-1+2, iYDiff/abs(iYDiff)+1};
    }
    else
    {
        piDirPrefs = {iYDiff/abs(iYDiff)+1, (iXDiff/abs(iXDiff))*-1+2};
    }

    for (int i=0; i<2; i++)
    {
        iNewDir = piDirPrefs[i];
        if (pObj->CanMoveInDirection(iNewDir))
        {
            return iNewDir;
        }
    }

    for (int i=1; i<5; i++)
    {
        iNewDir = piDirPrefs[1] + i;
        if (pObj->CanMoveInDirection(iNewDir))
        {
            return iNewDir;
        }
    }
    return -1;
}
