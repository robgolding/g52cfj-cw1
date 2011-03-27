#pragma once

#include "BaseEngine.h"
#include "TileManager.h"
#include "PacmanTileManager.h"

class PacmanMain :
    public BaseEngine
{
public:
    PacmanMain(void);
    ~PacmanMain(void);

    void SetupBackgroundBuffer();
    int InitialiseObjects();
    void DrawStrings();
    void GameAction();
    void MouseDown( int iButton, int iX, int iY );
    void KeyDown(int iKeyCode);

    // Get a reference to the current tile manager
    PacmanTileManager& GetTileManager() { return m_oTiles; }

private:
    PacmanTileManager m_oTiles;

public:
    // State number
    enum State { stateInit, stateMain, statePaused };

    void DrawScreen();
    void DrawChanges();

private:
    State m_state;
};

