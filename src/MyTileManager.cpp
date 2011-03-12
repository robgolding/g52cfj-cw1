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
