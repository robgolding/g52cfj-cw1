#include "header.h"

#include "PacmanMain.h"
#include "PacmanEnemy.h"

PacmanEnemy::PacmanEnemy(PacmanMain* pEngine, int iMapX, int iMapY)
: PacmanObject(pEngine, iMapX, iMapY)
{
}

void PacmanEnemy::Draw()
{
    // Do not draw if it should not be visible
    if ( !IsVisible() )
        return;

    //int iSize = 25;
    int iTick = m_pMainEngine->GetTime()/20; // 1 per 20ms
    int iFrame = iTick % 30;
    int iSize = 10 + iFrame;
    if ( iFrame > 15 )
        iSize = 10 + (30-iFrame);

    m_pMainEngine->DrawScreenOval(
            m_iCurrentScreenX - iSize,
            m_iCurrentScreenY - iSize,
            m_iCurrentScreenX + iSize-1,
            m_iCurrentScreenY + iSize-1,
            0x00ffff );

    // Store the position at which the object was last drawn
    // You MUST do this to ensure that the screen is updated when only
    // drawing movable objects
    // This tells the system where to 'undraw' the object from
    StoreLastScreenPositionAndUpdateRect();
}

void PacmanEnemy::HandleMovementFinished(int iCurrentTime)
{
    PacmanTileManager& tm = m_pMainEngine->GetTileManager();

    // Handle any tile that we just moved onto
    switch ( tm.GetValue( m_iMapX, m_iMapY ) )
    {
    case 2:
    case 3:
    case 4:
    case 5:
    case 6:
    case 7:
        tm.UpdateTile( m_pMainEngine, m_iMapX, m_iMapY,
        tm.GetValue( m_iMapX, m_iMapY ) + 1 );
        break;
    case 8:
        tm.UpdateTile( m_pMainEngine, m_iMapX, m_iMapY, 0 );
        break;
    }

    // Set off a new movement
    switch( rand() % 10 )
    {
        case 0: // Increase dir by 1
            m_iDir = ( m_iDir + 1 )%4;
            break;
        case 1: // Reduce dir by 1
            m_iDir = ( m_iDir + 3 )%4;
            break;
    }

    switch ( tm.GetValue(
                m_iMapX + GetXDiffForDirection(m_iDir),
                m_iMapY + GetYDiffForDirection(m_iDir) ) )
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
                    m_iMapX *50 + 25 + 25,
                    m_iMapY *50 + 25 + 40,
                    iCurrentTime,
                    iCurrentTime+400+rand()%200 );
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
