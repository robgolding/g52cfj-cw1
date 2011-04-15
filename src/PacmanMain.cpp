#include "header.h"
#include "BaseEngine.h"
#include "PacmanMain.h"
#include "PacmanObject.h"
#include "PacmanPlayer.h"
#include "PacmanEnemy.h"
#include "JPGImage.h"
#include "TileManager.h"


PacmanMain::PacmanMain(void)
: BaseEngine( 50 )
, m_state(stateInit)
, m_pPlayer(NULL)
, m_ppEnemies(NULL)
, m_iLives(5)
{
    printf("Init. Lives: %d\n", m_iLives);
}

PacmanMain::~PacmanMain(void)
{
}

void PacmanMain::SetupBackgroundBuffer()
{
    switch(m_state)
    {
        case stateInit:
            FillBackground(0xffff00);
            {
                char* data[] = {
                    "bbbbbbbbbbbbbbb",
                    "baeaeadadaeaeab",
                    "babcbcbcbcbibeb",
                    "badadgdadhdadhb",
                    "bgbcbcbcbibcbeb",
                    "badadadadadadab",
                    "bfbcbibcbcbcbeb",
                    "bahadadhdadadab",
                    "bfbcbcbibcbibeb",
                    "badadadadadadab",
                    "bbbbbbbbbbbbbbb"
                };

                m_oTiles.SetSize( 15, 11 );

                for ( int x = 0 ; x < 15 ; x++ )
                    for ( int y = 0 ; y < 11 ; y++ )
                        m_oTiles.SetValue( x, y, data[y][x]-'a' );

                for ( int y = 0 ; y < 11 ; y++ )
                {
                    for ( int x = 0 ; x < 15 ; x++ )
                        printf("%d ", m_oTiles.GetValue(x,y) );
                    printf("\n" );
                }

                m_oTiles.SetBaseTilesPositionOnScreen( 25, 40 );
            }
            return;
        case stateMain:
            FillBackground(0x000000);
            m_oTiles.DrawAllTiles(this,
                    this->GetBackground(),
                    0, 0, 14, 10
                    );
            break;
        case statePaused:
        case stateLifeLost:
            FillBackground(0x000000);
            m_oTiles.DrawAllTiles(this,
                    this->GetBackground(),
                    0, 0, 14, 10
                    );
            break;
    }
}

int PacmanMain::InitialiseObjects()
{
    // Record the fact that we are about to change the array - so it doesn't
    // get used elsewhere without reloading it
    DrawableObjectsChanged();

    // Destroy any existing objects
    DestroyOldObjects();

    int iNumEnemies = 4;

    // Create an array one element larger than the number of objects
    m_ppDisplayableObjects = new DisplayableObject*[iNumEnemies + 2];
    m_ppEnemies = new PacmanEnemy*[iNumEnemies+1];

    PacmanPlayer* pPlayer = new PacmanPlayer(this, 1, 1);
    m_pPlayer = pPlayer;

    m_ppDisplayableObjects[0] = pPlayer;

    for (int i=0; i<iNumEnemies; i++)
    {
        int iX = (m_oTiles.GetMapWidth() / 2) - (iNumEnemies / 2) + i;
        PacmanEnemy* pEnemy = new PacmanEnemy(this, iX, 5);
        m_ppEnemies[i] = pEnemy;
        m_ppDisplayableObjects[i+1] = pEnemy;
    }

    // Set the end of both arrays to NULL
    m_ppEnemies[iNumEnemies] = NULL;
    m_ppDisplayableObjects[iNumEnemies+1] = NULL;

    // NOTE: We also need to destroy the objects, but the method at the
    // top of this function will destroy all objects pointed at by the
    // array elements so we can ignore that here.

    return 0;
}

void PacmanMain::DrawStrings()
{
    switch( m_state )
    {
        case stateInit:
            CopyBackgroundPixels(0, 280, GetScreenWidth(), 40);
            DrawScreenString( 100, 300, "Initialised and waiting for SPACE", 0x0, NULL );
            SetNextUpdateRect(0, 280, GetScreenWidth(), 40);
            break;
        case stateMain:
            char buf[10]; // no more than 99 lives!
            CopyBackgroundPixels(0, 0, GetScreenWidth(), 30);
            sprintf(buf, "Lives: %d", m_iLives);
            DrawScreenString(25, 7, buf, 0xffffff, NULL);
            SetNextUpdateRect(0, 0, GetScreenWidth(), 30);
            break;
        case statePaused:
            CopyBackgroundPixels(0, 280, GetScreenWidth(), 40);
            DrawScreenString(200, 300, "Paused. Press SPACE to continue", 0xffffff, NULL );
            SetNextUpdateRect(0, 280, GetScreenWidth(), 40);
            break;
        case stateLifeLost:
            CopyBackgroundPixels(0, 280, GetScreenWidth(), 40);
            DrawScreenString(200, 300, "Life lost! Press SPACE to continue", 0xffffff, NULL );
            SetNextUpdateRect(0, 280, GetScreenWidth(), 50);
            break;
    }
}

/* Overridden GameAction to ensure that objects use the modified time */
void PacmanMain::GameAction()
{
    // If too early to act then do nothing
    if ( !TimeToAct() )
        return;

    // Don't act for another 10 ticks
    SetTimeToAct( 1 );

    // NEW SWITCH
    switch( m_state )
    {
    case stateInit:
    case statePaused:
    case stateLifeLost:
        break;
    case stateMain:
        // Only tell objects to move when not paused etc
        UpdateAllObjects( GetTime() );
        break;
    }
}

// Override to add a node at specified point
void PacmanMain::MouseDown( int iButton, int iX, int iY )
{
}

/*
Handle any key presses here.
Note that the objects themselves (e.g. player) may also check whether a key is pressed
*/
void PacmanMain::KeyDown(int iKeyCode)
{
    // NEW SWITCH
    switch ( iKeyCode )
    {
    case SDLK_ESCAPE: // End program when escape is pressed
        SetExitWithCode( 0 );
        break;
    case SDLK_SPACE: // SPACE Pauses
        switch( m_state )
        {
        case stateInit:
            // Go to state main
            m_state = stateMain;
            // Force redraw of background
            SetupBackgroundBuffer();
            // Redraw the whole screen now
            Redraw(true);
            break;
        case stateMain:
            // Go to state paused
            m_state = statePaused;
            // Force redraw of background
            SetupBackgroundBuffer();
            // Redraw the whole screen now
            Redraw(true);
            break;
        case statePaused:
        case stateLifeLost:
            // Go to state main
            m_state = stateMain;
            // Force redraw of background
            SetupBackgroundBuffer();
            // Redraw the whole screen now
            Redraw(true);
            break;
        } // End switch on current state
        break; // End of case SPACE
    }
}

int PacmanMain::GameInit()
{
    // do this the correct way round so InitialiseObjects can check the value
    // of the tiles for placing objects
    SetupBackgroundBuffer();
    InitialiseObjects();
    return 0;
}

/* Draw the changes to the screen.
Remove the changing objects, redraw the strings and draw the changing objects again.
 */
void PacmanMain::DrawChanges()
{
    // NEW IF
    if ( m_state == stateInit )
        return; // Do not draw objects if initialising

    // Remove objects from their old positions
    UndrawChangingObjects();
    // Draw the text for the user
    DrawStrings();
    // Draw objects at their new positions
    DrawChangingObjects();
}

void PacmanMain::DrawScreen()
{
    // First draw the background
    CopyAllBackgroundBuffer();

    // Then draw the text
    DrawStrings();

    if ( m_state == stateInit )
        return; // Do not draw objects if initialising

    // Then draw the changing objects
    DrawChangingObjects();
}

// A collision has been detected between the player `player' and enemy `enemy'
void PacmanMain::CollisionDetected(PacmanPlayer* player, PacmanEnemy* enemy)
{
    LoseLife();
}

void PacmanMain::LoseLife()
{
    --m_iLives; // so much faster than post-increment!
    PacmanEnemy* pEnemy;
    int i = 0;
    while ( (pEnemy = m_ppEnemies[i]) != NULL)
    {
        pEnemy->SetVisible(false);
        i++;
    }
    InitialiseObjects();
    Redraw(true);
    m_state = stateLifeLost;
}
