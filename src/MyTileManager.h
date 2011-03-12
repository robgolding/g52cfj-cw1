#ifndef MyTileManager_H
#define MyTileManager_H

#include "TileManager.h"


class MyTileManager :
    public TileManager
{
public:
    // Get the size of a tile - width
    virtual int GetTileWidth();

    // Get the size of a tile - height
    virtual int GetTileHeight();

    virtual int GetHeight();

    virtual int GetWidth();
};

#endif
