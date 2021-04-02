#define OLC_PGE_APPLICATION
#include "AntColonySimulation.h"

int main()
{
    AntColonySimulation antSim;
    if( antSim.Construct( 1400, 840, 1, 1 ) )
        antSim.Start();
    return 0;
}