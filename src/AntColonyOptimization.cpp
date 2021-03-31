#include "AntColonyOptimization.h"

AntColonyOptimization::AntColonyOptimization()
{
    /* initialize random seed */
    srand( unsigned int( time( NULL ) ) );

    sAppName = "AntColonyOptimization";
}

bool AntColonyOptimization::OnUserCreate()
{
    // Called once at the start, so create things here
    SetPixelMode( olc::Pixel::ALPHA );

    m_nestPos = olc::vf2d( ScreenWidth() / 2.0f , ScreenHeight() / 2.0f );
    
    m_pheromoneMap = PheromoneMap();
    reset( true );

    return true;
}

bool AntColonyOptimization::OnUserUpdate( float timeElapsed )
{
    if( GetKey( olc::Key::ESCAPE ).bPressed )
    {
        reset();
    }
    if( GetKey( olc::Key::K1 ).bPressed )
    {
        reset( true );
    }

    /* test */
    if( GetKey( olc::Key::F ).bPressed )
    {
        m_pheromoneMap.addPheromone( olc::vf2d( ( float )GetMouseX(), ( float )GetMouseY() ), true );
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

    for( auto &a : m_vAnts )
    {
        a.update( *this, timeElapsed );
    }

    m_pheromoneMap.update( timeElapsed );

    composeFrame();
    return true;
}

void AntColonyOptimization::composeFrame()
{
    Clear( olc::Pixel( 184, 134, 11 ) );

    m_pheromoneMap.draw( *this );

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

void AntColonyOptimization::reset( const bool bOnlyOneAnt )
{
    m_vFood.clear();
    m_vAnts.clear();
    m_pheromoneMap.reset();

    const float width   = ( float )ScreenWidth();
    const float height  = ( float )ScreenHeight();

    if( true == bOnlyOneAnt )
    {
        Ant ant( olc::vf2d( width / 2, height / 2 ), 40, m_vFood, m_nestPos, width, height );
        m_vAnts.push_back( ant );
    }
    else
    {
        for( int i = 0; i < 2500; ++i )
        {
            //Ant ant( olc::vf2d( ( float )( rand() % ScreenWidth() ), ( float )( rand() % ScreenHeight() ) ), 20, m_vFood, m_nestPos, width, height );
            //Ant ant( olc::vf2d( width / 2, height / 2 ), 20, m_vFood, m_nestPos, width, height );
            Ant ant( m_nestPos + olc::vf2d( ( float )( -20 + rand() % 40 ), ( float )( -20 + rand() % 40 ) ), 15, m_vFood, m_nestPos, width, height );
            m_vAnts.push_back( ant );
        }
    }    
}
