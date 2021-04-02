#include "AntColonySimulation.h"

#define MEASURE_EXECUTION_TIMES 0

#if MEASURE_EXECUTION_TIMES
#include <chrono>  // for high_resolution_clock
#endif
AntColonySimulation::AntColonySimulation()
{
    /* initialize random seed */
    srand( unsigned int( time( NULL ) ) );

    sAppName = "Ant Colony Simulation";
}

AntColonySimulation::~AntColonySimulation()
{
    if( m_pHomePheromones )
    {
        delete m_pHomePheromones;
        m_pHomePheromones = nullptr;
    }
    if( m_pHomePheromones )
    {
        delete m_pHomePheromones;
        m_pHomePheromones = nullptr;
    }
}

bool AntColonySimulation::OnUserCreate()
{
    // Called once at the start, so create things here
    SetPixelMode( olc::Pixel::ALPHA );

    m_nestPos = olc::vf2d( ScreenWidth() / 2.0f , ScreenHeight() / 2.0f );
    
    m_pHomePheromones = new PheromoneMap( ScreenWidth(), ScreenHeight(), true );
    m_pFoodPheromones = new PheromoneMap( ScreenWidth(), ScreenHeight(), false );
    //m_pheromoneMap = PheromoneMap();
    reset( true );

    return true;
}

bool AntColonySimulation::OnUserUpdate( float timeElapsed )
{
#if MEASURE_EXECUTION_TIMES
    auto startOnUserUpdate = std::chrono::high_resolution_clock::now();
#endif

    if( GetKey( olc::Key::ESCAPE ).bPressed )
    {
        reset();
    }
    if( GetKey( olc::Key::K1 ).bPressed )
    {
        reset( true );
    }

    /* enable/disable pheromones drawing */
    if( GetKey( olc::Key::F ).bPressed )
    {
        m_bDrawPheromones = !m_bDrawPheromones;
    }

    /* left mouse click -> add food */
    if( GetMouse( 0 ).bPressed )
    {
        m_vFood.push_back( olc::vf2d( ( float )GetMouseX(), ( float )GetMouseY() ) );
    }

    /* right mouse click -> change nest position */
    if( GetMouse( 1 ).bPressed )
    {
        m_nestPos = olc::vf2d( ( float )GetMouseX(), ( float )GetMouseY() );
        for( auto &a : m_vAnts )
        {
            a.setNestPos( m_nestPos );
        }
    }

#if MEASURE_EXECUTION_TIMES
    auto startUpdateAnts = std::chrono::high_resolution_clock::now();
#endif
    for( auto &a : m_vAnts )
    {
        a.update( timeElapsed );
    }
#if MEASURE_EXECUTION_TIMES
    system( "cls" );
    auto finishUpdateAnts = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> elapsedUpdateAnts = finishUpdateAnts - startUpdateAnts;
#endif

#if MEASURE_EXECUTION_TIMES
    auto startUpdatePheromones = std::chrono::high_resolution_clock::now();
#endif

#if 0
    m_timeSinceLastPheromonesUpdate += timeElapsed;
    if( m_timeSinceLastPheromonesUpdate > m_updateIntervalPheromones )
    {
        m_pHomePheromones->update( m_timeSinceLastPheromonesUpdate );
        m_pFoodPheromones->update( m_timeSinceLastPheromonesUpdate );
        m_timeSinceLastPheromonesUpdate = 0.0;
    }
#else
    m_pHomePheromones->update( timeElapsed );
    m_pFoodPheromones->update( timeElapsed );
#endif

#if MEASURE_EXECUTION_TIMES
    auto finishUpdatePheromones = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> elapsedUpdatePheromones = finishUpdatePheromones - startUpdatePheromones;
#endif

#if MEASURE_EXECUTION_TIMES
    auto startComposeFrame = std::chrono::high_resolution_clock::now();
#endif

    composeFrame();

#if MEASURE_EXECUTION_TIMES
    std::cout << "\tupdate ants:\t\t" << elapsedUpdateAnts.count() << " ms\n";
    std::cout << "\tupdate pheromones:\t" << elapsedUpdatePheromones.count() << " ms\n";
    auto finishComposeFrame = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> elapsedComposeFrame = finishComposeFrame - startComposeFrame;
    std::cout << "\tcomposeFrame:\t\t" << elapsedComposeFrame.count() << " ms\n";

    auto finishOnUserUpdate = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> elapsed = finishOnUserUpdate - startOnUserUpdate;
    std::cout << "OnUserUpdate: " << elapsed.count() << " ms\n";
#endif

    return true;
}

void AntColonySimulation::composeFrame()
{
    Clear( olc::Pixel( 184, 134, 11 ) );

    if( true == m_bDrawPheromones )
    {
        m_pHomePheromones->draw( *this );
        m_pFoodPheromones->draw( *this );
    }

    FillCircle( m_nestPos, 20, olc::Pixel( 139, 69, 19 ) );

    for( const auto &f : m_vFood )
    {
        FillCircle( f, 4, olc::GREEN );
    }

    for( const auto &a : m_vAnts )
    {
        a.draw( *this );
    }
}

void AntColonySimulation::reset( const bool bOnlyOneAnt )
{
    m_vFood.clear();
    m_vAnts.clear();
    m_pHomePheromones->reset();
    m_pFoodPheromones->reset();
    m_timeSinceLastPheromonesUpdate = 0.0f;

    const float width   = ( float )ScreenWidth();
    const float height  = ( float )ScreenHeight();

    /* random ants */
    if( true == bOnlyOneAnt )
    {
        Ant ant( m_nestPos, 20, m_vFood, m_nestPos, width, height, m_pHomePheromones, m_pFoodPheromones );
        m_vAnts.push_back( ant );
    }
    else
    {
        for( int i = 0; i < 200; ++i )
        {
            Ant ant( m_nestPos + olc::vf2d( ( float )( -20 + rand() % 40 ), ( float )( -20 + rand() % 40 ) ), 20, m_vFood, m_nestPos, width, height, m_pHomePheromones, m_pFoodPheromones );
            m_vAnts.push_back( ant );
        }
    }

    /* food */
    const olc::vf2d center( 300, 200 );
    const int radius = 80;
    for( int i = 0; i < 150; ++i )
    {
        const float rndAngle = ( float )rand() / ( float )RAND_MAX * 6.28318f;
        olc::vf2d rndPntUnitCircle;
        rndPntUnitCircle.x = sinf( rndAngle );
        rndPntUnitCircle.y = -cosf( rndAngle );

        m_vFood.push_back( olc::vf2d( rndPntUnitCircle *= ( float )( rand() % radius ) ) + center );
    }
}
