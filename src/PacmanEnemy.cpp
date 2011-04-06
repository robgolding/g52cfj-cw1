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

void PacmanEnemy::DoUpdate( int iCurrentTime )
{
    m_iPreviousScreenX = m_iCurrentScreenX;
    m_iPreviousScreenY = m_iCurrentScreenY;

    // Iterate through the objects
    // We are looking for one which is too close to us
    DisplayableObject* pObject;
    for (
            int iObjectId = 0;
            (pObject = m_pMainEngine->GetDisplayableObject(iObjectId)) != NULL;
            iObjectId++
        )
    {
        if ( pObject == this ) // This is us, skip it
            continue;
        // If you need to cast to the sub-class type, you must use
        // dynamic_cast, see lecture 19
        // We are just using base class parts
        int iXDiff = pObject->GetXCentre() - m_iCurrentScreenX;
        int iYDiff = pObject->GetYCentre() - m_iCurrentScreenY;

        // Estimate the size - by re-calculating it
        int iTick = iCurrentTime/20; // 1 per 20ms
        int iFrame = iTick % 30;
        int iSize = 10 + iFrame;
        if ( iFrame > 15 )
            iSize = 10 + (30-iFrame);
        int iSizeOther = iSize; // Assume both the same size

        /*
        // Pythagorus' theorum:
        if ( ((iXDiff*iXDiff)+(iYDiff*iYDiff))
                < ((iSizeOther+iSize)*(iSizeOther+iSize)) )
        {
            // Move us to 1,1 and set direction right
            m_iMapX = 1+rand()%13;
            m_iMapY = 1+(rand()%2)*8;
            m_iDir = 1; // Face right
            m_oMover.Setup(
                    m_iMapX *50 + 25 + 25, //m_iCurrentScreenX,
                    m_iMapY *50 + 25 + 40, //m_iCurrentScreenY,
                    m_iMapX *50 + 25 + 25,
                    m_iMapY *50 + 25 + 40,
                    iCurrentTime,
                    iCurrentTime+400+rand()%200 );
            // Ask the mover where the object should be
            m_oMover.Calculate( iCurrentTime );
            m_iCurrentScreenX = m_oMover.GetX();
            m_iCurrentScreenY = m_oMover.GetY();
            // Ensure that the object gets redrawn on the display, if something changed
            RedrawObjects();
            return;
        }
        */
    }

    // If movement has finished
    if ( m_oMover.HasMovementFinished(iCurrentTime) )
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

    // If making a move then do the move
    if ( !m_oMover.HasMovementFinished(iCurrentTime) )
    {
        // Ask the mover where the object should be
        m_oMover.Calculate( iCurrentTime );
        m_iCurrentScreenX = m_oMover.GetX();
        m_iCurrentScreenY = m_oMover.GetY();
    }

    // Ensure that the object gets redrawn on the display, if something changed
    RedrawObjects();
}
