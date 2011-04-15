#pragma once
#include "MovementPosition.h"

class PacmanMover : public MovementPosition
{
    public:
        void Reverse(int iCurrentTime);
};
