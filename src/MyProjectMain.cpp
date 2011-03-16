#if defined(_MSC_VER)
#include "SDL.h"
#else
#include "SDL/SDL.h"
#endif

#include "BaseEngine.h"

#include "MyProjectMain.h"

#include "JPGImage.h"

#include "TileManager.h"

#include "DisplayableObject.h"

class SimpleShape : public DisplayableObject
{
private:
    double m_dSX;
    double m_dSY;
    double m_dX;
    double m_dY;

public:
    SimpleShape(MyProjectMain* pEngine )
        : DisplayableObject( pEngine )
    {
        // The ball coordinate will be the centre of the ball
        // Because we start drawing half the size to the top-left.
        m_iStartDrawPosX = -50;
        m_iStartDrawPosY = -50;
        // Record the ball size as both height and width
        m_iDrawWidth = 100;
        m_iDrawHeight = 100;
        // Just put it somewhere initially
        m_dX = m_iPreviousScreenX = m_iCurrentScreenX = m_iDrawWidth;
        m_dY = m_iPreviousScreenY = m_iCurrentScreenY = m_iDrawHeight;
        // Speed
        m_dSX = 1;
        m_dSY = 1;
        // And make it visible
        SetVisible(true);
    }

    void Draw()
    {
        GetEngine()->DrawScreenOval( 
            m_iCurrentScreenX - m_iDrawWidth/2 + 1,
            m_iCurrentScreenY - m_iDrawHeight/2 + 1,
            m_iCurrentScreenX + m_iDrawWidth/2 -1,
            m_iCurrentScreenY + m_iDrawHeight/2 -1,
            0xffff00 );
        // Store drawing position so it can be removed again
        StoreLastScreenPositionAndUpdateRect();
    }

    void DoUpdate( int iCurrentTime )
    {
        // Change speed if player presses a key
        if ( GetEngine()->IsKeyPressed( SDLK_UP ) )
            m_dSY -= 0.001;
        if ( GetEngine()->IsKeyPressed( SDLK_DOWN ) )
            m_dSY += 0.001;
        if ( GetEngine()->IsKeyPressed( SDLK_LEFT ) )
            m_dSX -= 0.001;
        if ( GetEngine()->IsKeyPressed( SDLK_RIGHT ) )
            m_dSX += 0.001;
        if ( GetEngine()->IsKeyPressed( SDLK_SPACE ) )
            m_dSX = m_dSY = 0;

        // Alter position for speed
        m_dX += m_dSX;
        m_dY += m_dSY;

        // Check for bounce
        if ( (m_dX+m_iStartDrawPosX) < 0 )
        {
            m_dX = - m_iStartDrawPosX;
            if ( m_dSX < 0 )
                m_dSX = -m_dSX;
        }
        if ( (m_dX+m_iStartDrawPosX+m_iDrawWidth) > (GetEngine()->GetScreenWidth()-1) )
        {
            m_dX = GetEngine()->GetScreenWidth() -1 - m_iStartDrawPosX - m_iDrawWidth;
            if ( m_dSX > 0 )
                m_dSX = -m_dSX;
        }
        if ( (m_dY+m_iStartDrawPosY) < 0 )
        {
            m_dY = -m_iStartDrawPosY;
            if ( m_dSY < 0 )
                m_dSY = -m_dSY;
        }
        if ( (m_dY+m_iStartDrawPosY+m_iDrawHeight) > (GetEngine()->GetScreenHeight()-1) )
        {
            m_dY = GetEngine()->GetScreenHeight() -1 - m_iStartDrawPosY - m_iDrawHeight;
            if ( m_dSY > 0 )
                m_dSY = -m_dSY;
        }

        // Set current position
        m_iCurrentScreenX = (int)(m_dX+0.5);
        m_iCurrentScreenY = (int)(m_dY+0.5);

        // Ensure that the object gets redrawn on the display, if something changed
        RedrawObjects();
    }
};


/*
Do any setup of back buffer prior to locking the screen buffer
Basically do the drawing of the background in here and it'll be copied to the screen for you as needed
*/
void MyProjectMain::SetupBackgroundBuffer()
{
    FillBackground( 0 );

    int tileWidth = GetScreenWidth() / m_oTileManager.GetTileWidth();
    int tileHeight = GetScreenHeight() / m_oTileManager.GetTileHeight();

    m_oTileManager.SetSize(tileWidth, tileHeight);

    // Specify the screen x,y of top left corner
    m_oTileManager.SetBaseTilesPositionOnScreen( 0, 0 );


    for ( int x = 0 ; x < tileWidth ; x++ )
        for ( int y = 0 ; y < tileHeight ; y++ )
            m_oTileManager.SetValue( x, y, rand()%41 );

    // Tell it to draw tiles from x1,y1 to x2,y2 in tile array,
    // to the background of this screen
    m_oTileManager.DrawAllTiles( this, this->GetBackground(), 0, 0,
            m_oTileManager.GetWidth() - 1,
            m_oTileManager.GetHeight() - 1
    );

}
/*
In here you need to create any movable objects that you wish to use.
Sub-classes need to implement this function.
*/
int MyProjectMain::InitialiseObjects()
{
    // Record the fact that we are about to change the array - so it doesn't get used elsewhere without reloading it
    DrawableObjectsChanged();

    // Destroy any existing objects
    DestroyOldObjects();

    // Create an array one element larger than the number of objects that you want.
    m_ppDisplayableObjects = new DisplayableObject*[2];


    // You MUST set the array entry after the last one that you create to NULL, so that the system knows when to stop.
    // i.e. The LAST entry has to be NULL. The fact that it is NULL is used in order to work out where the end of the array is.
    m_ppDisplayableObjects[0] = new SimpleShape(this);
    m_ppDisplayableObjects[1] = NULL;

    return 0;
}








/* Draw text labels */
void MyProjectMain::DrawStrings()
{
    //CopyBackgroundPixels( 0, 0, GetScreenWidth(), 30 );
    //DrawScreenString( 150, 10, "Example text", 0xffffff, NULL );
    //SetNextUpdateRect( 0/*X*/, 0/*Y*/, GetScreenWidth(), 30/*Height*/ );
}


/* Overridden GameAction to ensure that objects use the modified time */
void MyProjectMain::GameAction()
{
    // If too early to act then do nothing
    if ( !TimeToAct() )
        return;

    // Don't act for another 10 ticks
    SetTimeToAct( 1 );

    UpdateAllObjects( GetTime() );
}


// Override to add a node at specified point
void MyProjectMain::MouseDown( int iButton, int iX, int iY )
{
    //SetupBackgroundBuffer();
    //Redraw(true); // Force total redraw
}

/*
Handle any key presses here.
Note that the objects themselves (e.g. player) may also check whether a key is pressed
*/
void MyProjectMain::KeyDown(int iKeyCode)
{
    switch ( iKeyCode )
    {
    case SDLK_ESCAPE: // End program when escape is pressed
        SetExitWithCode( 0 );
        break;
    case SDLK_SPACE: // SPACE Pauses
        break;
    }
}
