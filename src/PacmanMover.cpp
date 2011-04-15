#include "PacmanMover.h"

void PacmanMover::Reverse(int iCurrentTime)
{
    Swap(m_iStartX,m_iEndX);
    Swap(m_iStartY,m_iEndY);
    int iTimePassed = iCurrentTime - m_iStartTime;
    int iTimeLeft = m_iEndTime - iCurrentTime;
    m_iStartTime = iCurrentTime - iTimeLeft;
    m_iEndTime = iCurrentTime + iTimePassed;
}
