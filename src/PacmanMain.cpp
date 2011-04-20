#include "header.h"
#include "BaseEngine.h"
#include "PacmanMain.h"
#include "PacmanObject.h"
#include "PacmanPlayer.h"
#include "PacmanEnemy.h"
#include "JPGImage.h"
#include "TileManager.h"

#include <iostream>
#include <fstream>
using namespace std;

PacmanMain::PacmanMain(void)
: BaseEngine(50)
, m_state(stateInit)
, m_pPlayer(NULL)
, m_ppEnemies(NULL)
, m_iLives(5)
, m_iNumEnemies(0)
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
            return;
        case stateMain:
            FillBackground(0x000000);
            m_oTiles.DrawAllTiles(this,
                    this->GetBackground(),
                    0, 0, m_oTiles.GetMapWidth() - 1, m_oTiles.GetMapHeight() - 1
                    );
            break;
        case statePaused:
        case stateLifeLost:
        case stateGameOver:
            FillBackground(0x000000);
            m_oTiles.DrawAllTiles(this,
                    this->GetBackground(),
                    0, 0, m_oTiles.GetMapWidth() - 1, m_oTiles.GetMapHeight() - 1
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

    // Create an array one element larger than the number of objects
    m_ppDisplayableObjects = new DisplayableObject*[m_iNumEnemies + 2];

    m_ppDisplayableObjects[0] = m_pPlayer;

    for (int i=0; i<m_iNumEnemies; i++)
    {
        m_ppDisplayableObjects[i+1] = m_ppEnemies[i];
    }

    // Set the end of the array to NULL
    m_ppDisplayableObjects[m_iNumEnemies+1] = NULL;

    // NOTE: We also need to destroy the objects, but the method at the
    // top of this function will destroy all objects pointed at by the
    // array elements so we can ignore that here.
    return 0;
}

int PacmanMain::LoadMapFromFile(char* filename)
{
    int width = -1;
    int height = 0;
    char line[255];
    char* data[255];
    ifstream in(filename);

    m_iNumEnemies = 0;
    m_ppEnemies = new PacmanEnemy*[10];

    if (in.is_open())
    {
        while (in.good())
        {
            in.getline(line, 255);
            if (strlen(line) == 0)
                continue;
            data[height] = new char[strlen(line)+1];
            strcpy(data[height], line);
            ++height;
            if (width == -1 || strlen(line) < width)
                width = strlen(line);
        }
        in.close();
    }
    else
    {
        printf("Cannot open map '%s'.\n", filename);
        return 1;
    }

    m_oTiles.SetBaseTilesPositionOnScreen(25, 40);
    m_oTiles.SetSize(width, height);

    char value;
    for (int y=0; y<height; ++y)
    {
        for (int x=0; x<width; ++x)
        {
            value = data[y][x];
            if (value == 'w')
                m_oTiles.SetValue(x, y, 1); // wall
            if (value == ' ')
                m_oTiles.SetValue(x, y, 0); // blank tile
            if (value == 'p')
                m_oTiles.SetValue(x, y, 2); // pellet
            if (value == 'e')
                m_ppEnemies[m_iNumEnemies++] = new PacmanEnemy(this, x, y);
            printf("%c ", data[y][x]);
        }
        printf("\n");
    }

    return 0;
}

void PacmanMain::DrawStrings()
{
    switch( m_state)
    {
        case stateInit:
            CopyBackgroundPixels(0, 280, GetScreenWidth(), 40);
            DrawScreenString(100, 300, "Initialised and waiting for SPACE", 0x0, NULL);
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
            DrawScreenString(200, 300, "Paused. Press SPACE to continue", 0xffffff, NULL);
            SetNextUpdateRect(0, 280, GetScreenWidth(), 40);
            break;
        case stateLifeLost:
            CopyBackgroundPixels(0, 280, GetScreenWidth(), 40);
            DrawScreenString(200, 300, "Life lost! Press SPACE to continue", 0xffffff, NULL);
            SetNextUpdateRect(0, 280, GetScreenWidth(), 50);
            break;
        case stateGameOver:
            CopyBackgroundPixels(0, 280, GetScreenWidth(), 40);
            DrawScreenString(50, 300, "Game over! Press SPACE to start again or ESC to exit", 0xffffff, NULL);
            SetNextUpdateRect(0, 280, GetScreenWidth(), 50);
            break;
    }
}

/* Overridden GameAction to ensure that objects use the modified time */
void PacmanMain::GameAction()
{
    // If too early to act then do nothing
    if (!TimeToAct())
        return;

    // Don't act for another 10 ticks
    SetTimeToAct(1);

    // NEW SWITCH
    switch(m_state)
    {
    case stateInit:
    case statePaused:
    case stateLifeLost:
    case stateGameOver:
        break;
    case stateMain:
        // Only tell objects to move when not paused etc
        UpdateAllObjects(GetTime());
        break;
    }
}

// Override to add a node at specified point
void PacmanMain::MouseDown(int iButton, int iX, int iY)
{
}

/*
Handle any key presses here.
Note that the objects themselves (e.g. player) may also check whether a key is pressed
*/
void PacmanMain::KeyDown(int iKeyCode)
{
    // NEW SWITCH
    switch (iKeyCode)
    {
    case SDLK_ESCAPE: // End program when escape is pressed
        SetExitWithCode(0);
        break;
    case SDLK_SPACE: // SPACE Pauses
        switch(m_state)
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
        case stateGameOver:
            m_iLives = 5;
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

    LoadMapFromFile("map.txt");
    m_pPlayer = new PacmanPlayer(this, 1, 1);

    InitialiseObjects();
    return 0;
}

/* Draw the changes to the screen.
Remove the changing objects, redraw the strings and draw the changing objects again.
 */
void PacmanMain::DrawChanges()
{
    // NEW IF
    if (m_state == stateInit)
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

    if (m_state == stateInit)
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
    while ((pEnemy = m_ppEnemies[i]) != NULL)
    {
        pEnemy->SetVisible(false);
        i++;
    }
    InitialiseObjects();
    Redraw(true);
    if (m_iLives > 0)
        m_state = stateLifeLost;
    else
        m_state = stateGameOver;
}
