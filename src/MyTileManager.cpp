#include "BaseEngine.h"

#include "MyTileManager.h"

int MyTileManager::GetTileHeight()
{
    return 10;
}

int MyTileManager::GetTileWidth()
{
    return 10;
}

int MyTileManager::GetHeight()
{
    return m_iMapHeight;
}

int MyTileManager::GetWidth()
{
    return m_iMapWidth;
}

void MyTileManager::DrawTile(BaseEngine* pEngine, SDL_Surface* pSurface,
        int iTileX, int iTileY )
{
    /*
    DrawTile( pEngine, pSurface, iTileX, iTileY,
            m_iBaseScreenX + iTileX*GetTileWidth(),
            m_iBaseScreenY + iTileY*GetTileHeight() );
            */
    printf("Draw tile called");
}

void MyTileManager::DrawTileAt(BaseEngine* pEngine, SDL_Surface* pSurface,
        int iMapX, int iMapY, int iStartPositionScreenX,
        int iStartPositionScreenY)
{
    printf("Drawing tile!");
    unsigned int iColour = 0xFF0000;
    pEngine->DrawRectangle( 
            iStartPositionScreenX,
            iStartPositionScreenY, 
            iStartPositionScreenX + GetTileWidth() - 1,
            iStartPositionScreenY + GetTileHeight() - 1,
            iColour,
            pSurface );
}
