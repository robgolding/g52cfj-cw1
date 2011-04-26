#include "header.h"
#include "BaseEngine.h"
#include "PacmanMain.h"
#include "PacmanObject.h"
#include "PacmanPlayer.h"
#include "PacmanEnemy.h"
#include "PacmanAI.h"
#include "JPGImage.h"
#include "TileManager.h"

#include <iostream>
#include <fstream>
using namespace std;

PacmanMain::PacmanMain(void)
: BaseEngine(50)
, m_state(stateInit)
, m_prevState(stateMain)
, m_pPlayer(NULL)
, m_ppEnemies(NULL)
, m_iPauseDuration(0)
, m_iLives(5)
, m_iNumEnemies(0)
, m_iPoints(0)
, m_iPowerupRemaining(0)
{
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
        case statePowerup:
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
            for(int i=0; i<strlen(line); i++)
                if (line[i] == 'e')
                    ++m_iNumEnemies;
        }
        in.close();
    }
    else
    {
        printf("Cannot open map '%s'.\n", filename);
        return 1;
    }

    m_ppEnemies = new PacmanEnemy*[m_iNumEnemies+1];

    m_oTiles.SetBaseTilesPositionOnScreen(25, 40);
    m_oTiles.SetSize(width, height);

    char value;
    int iEnemyNumber = 0;
    for (int y=0; y<height; ++y)
    {
        for (int x=0; x<width; ++x)
        {
            value = data[y][x];
            if (value == 'w')
                m_oTiles.SetValue(x, y, 1); // wall
            if (value == ' ')
                m_oTiles.SetValue(x, y, 0); // blank tile
            if (value == '.')
                m_oTiles.SetValue(x, y, 2); // pellet
            if (value == 'o')
                m_oTiles.SetValue(x, y, 7); // powerup
            if (value == 'e')
                m_ppEnemies[iEnemyNumber++] = new PacmanEnemy(this, x, y, new PacmanAI(this));
            if (value == 't')
                m_oTiles.SetValue(x, y, 9); // teleport
            printf("%c ", data[y][x]);
        }
        printf("\n");
    }

    m_ppEnemies[iEnemyNumber] = NULL;

    return 0;
}

void PacmanMain::DrawStrings()
{
    switch(m_state)
    {
        case stateInit:
            CopyBackgroundPixels(0, 280, GetScreenWidth(), 40);
            DrawScreenString(170, 280, "Ready? Press SPACE to start", 0x0, NULL);
            SetNextUpdateRect(0, 280, GetScreenWidth(), 50);
            break;
        case stateMain:
        case statePowerup:
            char lives[10]; // no more than 99 lives!
            char points[15]; // no more than 1,000,000 points!
            sprintf(lives, "Lives: %d", m_iLives);
            sprintf(points, "Points: %d", m_iPoints);
            CopyBackgroundPixels(0, 7, GetScreenWidth(), 50);
            DrawScreenString(25, 7, lives, 0xffffff, NULL);
            DrawScreenString(600, 7, points, 0xffffff, NULL);
            SetNextUpdateRect(0, 7, GetScreenWidth(), 50);
            break;
        case statePaused:
            CopyBackgroundPixels(0, 300, GetScreenWidth(), 50);
            DrawScreenString(200, 300, "Paused. Press SPACE to continue", 0xffffff, NULL);
            SetNextUpdateRect(0, 300, GetScreenWidth(), 50);
            break;
        case stateLifeLost:
            CopyBackgroundPixels(0, 200, GetScreenWidth(), 150);
            DrawScreenString(300, 200, "Life lost!", 0xffffff, NULL);
            DrawScreenString(200, 300, "Press SPACE to continue", 0xffffff, NULL);
            SetNextUpdateRect(0, 200, GetScreenWidth(), 150);
            break;
        case stateGameOver:
            CopyBackgroundPixels(0, 200, GetScreenWidth(), 150);
            DrawScreenString(300, 200, "Game over!", 0xffffff, NULL);
            DrawScreenString(90, 300, "Press SPACE to start again or ESC to exit", 0xffffff, NULL);
            SetNextUpdateRect(0, 200, GetScreenWidth(), 150);
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
    SetTimeToAct(10);

    switch(m_state)
    {
    case stateInit:
        break;
    case statePaused:
    case stateLifeLost:
    case stateGameOver:
        m_iPauseDuration += 10;
        break;
    case statePowerup:
        m_iPowerupRemaining -= 10;
        if (m_iPowerupRemaining <= 0)
            m_state = stateMain;
    case stateMain:
        // Only tell objects to move when not paused etc
        UpdateAllObjects(GetModifiedTime());
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
        case statePowerup:
            // Go to state paused
            m_prevState = m_state;
            m_state = statePaused;
            // Force redraw of background
            SetupBackgroundBuffer();
            // Redraw the whole screen now
            Redraw(true);
            break;
        case statePaused:
            m_state = m_prevState;
            SetupBackgroundBuffer();
            Redraw(true);
            break;
        case stateLifeLost:
            m_state = stateMain;
            SetupBackgroundBuffer();
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

int PacmanMain::GetModifiedTime()
{
    return GetTime() - m_iPauseDuration;
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
    if (IsInPowerupState())
    {
        enemy->ResetPosition();
        Redraw(true);
    }
    else
        LoseLife();
}

void PacmanMain::LoseLife()
{
    m_state = stateLifeLost;
    --m_iLives; // so much faster than post-increment!
    PacmanObject* pObj;
    for (int i=0; m_ppDisplayableObjects[i] != NULL; i++)
    {
        //m_ppDisplayableObjects[i]->SetVisible(false);
        pObj = dynamic_cast<PacmanObject*>(m_ppDisplayableObjects[i]);
        pObj->ResetPosition();
    }
    Redraw(true);
    if (m_iLives < 1)
        GameOver();
}

void PacmanMain::GameOver()
{
    m_state = stateGameOver;
    GameInit();
    Redraw(true);
}

void PacmanMain::AtePellet(bool bIsPowerup)
{
    int iPoints = 10;

    if (m_state == statePowerup)
        iPoints += 10;

    if (bIsPowerup)
    {
        iPoints += 40;
        m_state = statePowerup;
        m_iPowerupRemaining = 5000;
    }

    m_iPoints += iPoints;
}

bool PacmanMain::IsInPowerupState()
{
    return m_state == statePowerup;
}

int PacmanMain::GetPowerupRemaining()
{
    return m_iPowerupRemaining;
}
