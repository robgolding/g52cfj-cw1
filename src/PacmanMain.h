#pragma once

#include "BaseEngine.h"
#include "TileManager.h"
#include "PacmanTileManager.h"
#include "PacmanPlayer.h"
#include "PacmanEnemy.h"

class PacmanMain : public BaseEngine
{
public:
    PacmanMain(void);
    ~PacmanMain(void);

    void SetupBackgroundBuffer();
    int InitialiseObjects();
    virtual int LoadMapFromFile(char* filename);

    void DrawStrings();
    void GameAction();
    void MouseDown(int iButton, int iX, int iY);
    void KeyDown(int iKeyCode);
    int GameInit();

    void CollisionDetected(PacmanPlayer* player, PacmanEnemy* enemy);
    void LoseLife();

    PacmanTileManager& GetTileManager() { return m_oTiles; }
    PacmanPlayer& GetPlayer() { return *m_pPlayer; }

    enum State {stateInit, stateMain, statePaused, stateLifeLost, stateGameOver};

    void DrawScreen();
    void DrawChanges();

private:
    PacmanTileManager m_oTiles;
    State m_state;
    PacmanPlayer* m_pPlayer;
    PacmanEnemy** m_ppEnemies;
    int m_iLives;
    int m_iNumEnemies;
};

