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
            0xffff00 );

    // Store the position at which the object was last drawn
    // You MUST do this to ensure that the screen is updated when only drawing movable objects
    // This tells the system where to 'undraw' the object from
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
    // Iterate through the objects
    // We are looking for one which is too close to us
    DisplayableObject* pObject;
    for ( int iObjectId = 0 ;
            (pObject = m_pMainEngine->GetDisplayableObject( iObjectId )
            ) != NULL ;
            iObjectId++ )
    {
        if ( pObject == this ) // This is us, skip it
            continue;
        // If you need to cast to the sub-class type, you must use dynamic_cast, see lecture 19
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

        // Pythagorus' theorum:
        if ( ((iXDiff*iXDiff)+(iYDiff*iYDiff))
                < ((iSizeOther+iSize)*(iSizeOther+iSize)) )
        {
            PacmanEnemy* enemy = dynamic_cast<PacmanEnemy*>(pObject);
            if (enemy != NULL && enemy->IsVisible())
                m_pMainEngine->CollisionDetected(this, enemy);
            else
                printf("Collided with something that isn't a (visible) PacmanEnemy!\n");
        }
    }
}

void PacmanPlayer::HandleMovementFinished(int iCurrentTime)
{
    PacmanTileManager& tm = m_pMainEngine->GetTileManager();

    // Handle the tile that we just moved onto
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
                m_iMapX *50 + 25 + 25,
                m_iMapY *50 + 25 + 40,
                iCurrentTime,
                iCurrentTime+500);
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
        m_iDir = m_iNextDir = iDirection;
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
