#pragma once

#include "TileManager.h"

class PacmanTileManager :
    public TileManager
{
public:
    PacmanTileManager(void);
    ~PacmanTileManager(void);

    virtual int GetTileWidth();
    virtual int GetTileHeight();
    virtual int GetMapWidth();
    virtual int GetMapHeight();
    virtual int GetBaseScreenX();
    virtual int GetBaseScreenY();
    virtual void DrawTileAt(BaseEngine* pEngine, SDL_Surface* pSurface,
        int iMapX, int iMapY,
        int iStartPositionScreenX, int iStartPositionScreenY);
};
