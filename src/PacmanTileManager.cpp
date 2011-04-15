#include "header.h"

#include "PacmanTileManager.h"

PacmanTileManager::PacmanTileManager(void)
{
}

PacmanTileManager::~PacmanTileManager(void)
{
}

int PacmanTileManager::GetTileWidth()
{
    return 50;
}

int PacmanTileManager::GetTileHeight()
{
    return 50;
}

int PacmanTileManager::GetMapWidth()
{
    return m_iMapWidth;
}

int PacmanTileManager::GetMapHeight()
{
    return m_iMapHeight;
}

void PacmanTileManager::DrawTileAt(BaseEngine* pEngine, SDL_Surface* pSurface,
        int iMapX, int iMapY, int iStartPositionScreenX,
        int iStartPositionScreenY)
{
    switch( GetValue(iMapX,iMapY) )
    {
        case 0:
        case 1:
            pEngine->DrawRectangle(
                    iStartPositionScreenX,
                    iStartPositionScreenY,
                    iStartPositionScreenX + GetTileWidth() - 1,
                    iStartPositionScreenY + GetTileHeight() - 1,
                    pEngine->GetColour( GetValue(iMapX,iMapY) ),
                    pSurface );
            break;
        case 2:
        case 3:
        case 4:
        case 5:
        case 6:
        case 7:
        case 8:
            pEngine->DrawRectangle(
                    iStartPositionScreenX,
                    iStartPositionScreenY,
                    iStartPositionScreenX + GetTileWidth() - 1,
                    iStartPositionScreenY + GetTileHeight() - 1,
                    pEngine->GetColour( 0 ),
                    pSurface );
            pEngine->DrawOval(
                    iStartPositionScreenX+GetValue(iMapX,iMapY)*2+5,
                    iStartPositionScreenY+GetValue(iMapX,iMapY)*2+5,
                    iStartPositionScreenX + GetTileWidth() - GetValue(iMapX,iMapY)*2 -6,
                    iStartPositionScreenY + GetTileHeight() - GetValue(iMapX,iMapY)*2 -6,
                    pEngine->GetColour( GetValue(iMapX,iMapY) ),
                    pSurface );
            break;
    }
}
