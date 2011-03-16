#include "BaseEngine.h"

#include "MyTileManager.h"

int MyTileManager::GetTileHeight()
{
    return 40;
}

int MyTileManager::GetTileWidth()
{
    return 40;
}

int MyTileManager::GetHeight()
{
    return m_iMapHeight;
}

int MyTileManager::GetWidth()
{
    return m_iMapWidth;
}

void MyTileManager::DrawTileAt(BaseEngine* pEngine, SDL_Surface* pSurface,
        int iMapX, int iMapY, int iStartPositionScreenX,
        int iStartPositionScreenY)
{
    pEngine->DrawRectangle( 
        iStartPositionScreenX,
        iStartPositionScreenY, 
        iStartPositionScreenX + GetTileWidth() - 1,
        iStartPositionScreenY + GetTileHeight() - 1,
        pEngine->GetColour( GetValue(iMapX,iMapY) ),
        pSurface);
}
