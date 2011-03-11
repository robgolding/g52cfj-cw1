#if defined(_MSC_VER)
#include "SDL.h"
#else
#include "SDL/SDL.h"
#endif


#include "templates.h"

// Needs one of the following #includes, to include the class definition
#include "BouncingBallMain.h"
#include "MyProjectMain.h"

#define BASE_SCREEN_WIDTH 800
#define BASE_SCREEN_HEIGHT 600



int main(int argc, char *argv[])
{
    // Needs just one of the two following lines:
    //BouncingBallMain oMain;
    MyProjectMain oMain;

    char buf[1024];
    sprintf( buf, "My Demonstration Program : Size %d x %d", BASE_SCREEN_WIDTH, BASE_SCREEN_HEIGHT );
    int iResult = oMain.Initialise( buf, BASE_SCREEN_WIDTH, BASE_SCREEN_HEIGHT, "Cornerstone Regular.ttf", 24 );
    iResult = oMain.MainLoop();
    oMain.Deinitialise();

    return iResult;
}
