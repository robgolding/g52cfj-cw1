#pragma once
#include "PacmanMain.h"
#include "PacmanObject.h"

class PacmanAI
{
    public:
        PacmanAI(PacmanMain* pEngine);
        int GetMove(PacmanObject* pObj, int iMapX, int iMapY);
    private:
        PacmanMain* m_pMainEngine;
};
