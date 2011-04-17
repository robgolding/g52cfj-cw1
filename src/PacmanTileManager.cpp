#include "header.h"

#include "PacmanTileManager.h"

#include <iostream>
#include <fstream>
using namespace std;

PacmanTileManager::PacmanTileManager(void)
{
}

PacmanTileManager::~PacmanTileManager(void)
{
}

int PacmanTileManager::LoadMapFromFile(char* filename)
{
    int width = -1;
    int height = 0;
    char line[255];
    char* data[255];
    ifstream in(filename);

    if (in.is_open())
    {
        while (in.good())
        {
            in.getline(line, 255);
            if (strlen(line) == 0)
                continue;
            data[height] = new char[strlen(line)+1];
            strcpy(data[height], line);
            ++height;
            if (width == -1 || strlen(line) < width)
                width = strlen(line);
        }
        in.close();
    }
    else
    {
        cout << "Cannot open file.\n";
        return 1;
    }

    SetBaseTilesPositionOnScreen(25, 40);
    SetSize(width, height);

    char value;
    for (int y=0; y<height; ++y)
    {
        for (int x=0; x<width; ++x)
        {
            value = data[y][x] - 'a';
            SetValue(x, y, value);
            printf("%c ", data[y][x]);
        }
        printf("\n");
    }

    return 0;
}

int PacmanTileManager::GetTileWidth()
{
    return 25;
}

int PacmanTileManager::GetTileHeight()
{
    return 25;
}

int PacmanTileManager::GetBaseScreenX()
{
    return m_iBaseScreenX;
}

int PacmanTileManager::GetBaseScreenY()
{
    return m_iBaseScreenY;
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
