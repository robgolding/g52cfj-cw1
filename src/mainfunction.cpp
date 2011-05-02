#include "header.h"
#include "templates.h"
#include "BouncingBallMain.h"
#include "PacmanMain.h"

#include "sys/stat.h"

#define BASE_SCREEN_WIDTH 800
#define BASE_SCREEN_HEIGHT 600

int main(int argc, char *argv[])
{
#if defined(_MSC_VER)
#ifdef _MEMORYLEAKDEBUG
    _CrtSetDbgFlag ( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
    _CrtSetReportMode( _CRT_ERROR, _CRTDBG_MODE_DEBUG );
    //_CrtSetBreakAlloc(51);
#endif
#endif

    int iResult;

    {
        PacmanMain oMain;

        char* fmt = "maps/level_%d.txt";
        char filename[25];
        struct stat stFileInfo;

        for (int i=1; i<10; i++)
        {
            sprintf(filename, fmt, i);
            if (stat(filename, &stFileInfo) == 0)
            {
                oMain.AddLevel(filename);
            }
        }

        char buf[1024];
        sprintf(buf, "My Demonstration Program : Size %d x %d", BASE_SCREEN_WIDTH, BASE_SCREEN_HEIGHT);
        iResult = oMain.Initialise(buf, BASE_SCREEN_WIDTH, BASE_SCREEN_HEIGHT, "Cornerstone Regular.ttf", 24);
        iResult = oMain.MainLoop();
        oMain.Deinitialise();
    }

#if defined(_MSC_VER)
#ifdef _MEMORYLEAKDEBUG
    _CrtSetReportMode( _CRT_ERROR, _CRTDBG_MODE_DEBUG );
    _CrtDumpMemoryLeaks();
#endif
#endif

    return iResult;
}
