#define OLC_PGE_APPLICATION
#include "AntColonyOptimization.h"

int main()
{
    AntColonyOptimization game;
    if( game.Construct( 1400, 840, 1, 1 ) )
        game.Start();
    return 0;
}