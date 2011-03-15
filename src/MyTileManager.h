#ifndef MyTileManager_H
#define MyTileManager_H

#include "TileManager.h"


class MyTileManager : public TileManager
{
public:
    // Get the size of a tile - width
    virtual int GetTileWidth();

    // Get the size of a tile - height
    virtual int GetTileHeight();

    virtual int GetHeight();

    virtual int GetWidth();

    virtual void DrawTile(BaseEngine* pEngine, SDL_Surface* pSurface,
            int iTileX, int iTileY );

    virtual void DrawTileAt(BaseEngine* pEngine, SDL_Surface* pSurface,
            int iMapX, int iMapY, int iStartPositionScreenX,
            int iStartPositionScreenY );
};

#endif
