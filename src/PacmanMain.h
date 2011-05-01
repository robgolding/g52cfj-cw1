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

    void AddLevel(char* pMapName);

    void SetupBackgroundBuffer();
    int InitialiseObjects();
    virtual int LoadCurrentLevel();

    void DrawStrings();
    void GameAction();
    void MouseDown(int iButton, int iX, int iY);
    void KeyDown(int iKeyCode);
    int GameInit();
    int GetModifiedTime();

    void CollisionDetected(PacmanPlayer* player, PacmanEnemy* enemy);
    void LoseLife();
    void GameOver();
    void LevelCompleted();
    void GameCompleted();
    void AtePellet(bool bIsPowerup);
    bool IsInPowerupState();
    int GetPowerupRemaining();

    PacmanTileManager& GetTileManager() { return m_oTiles; }
    PacmanPlayer& GetPlayer() { return *m_pPlayer; }

    enum State {stateInit, stateMain, statePowerup, statePaused, stateLifeLost, stateLevelCompleted, stateGameCompleted, stateGameOver};

    void DrawScreen();
    void DrawChanges();

private:
    char** m_ppLevels;
    PacmanTileManager m_oTiles;
    State m_state;
    State m_prevState;
    PacmanPlayer* m_pPlayer;
    PacmanEnemy** m_ppEnemies;
    int m_iNumLevels;
    int m_iCurrentLevel;
    int m_iPauseDuration;
    int m_iLives;
    int m_iNumEnemies;
    int m_iNumPellets;
    int m_iPoints;
    int m_iPowerupRemaining;
};

