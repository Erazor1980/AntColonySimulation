#include "Pheromone.h"
#define _USE_MATH_DEFINES 
#include <math.h>


PheromoneMap::PheromoneMap( const int screenWidth, const int screenHeigth, const bool bHomePheromone )
{
    m_screenWidth       = ( float )screenWidth;
    m_screenHeight      = ( float )screenHeigth;
    m_bHomePheromones   = bHomePheromone;
    reset();
}

PheromoneMap::~PheromoneMap()
{
}

void PheromoneMap::addPheromone( const olc::vf2d& pos )
{
    Pheromone p;
    p.m_pos = pos;
    p.m_lifeTime = 0.0f;

    m_vPheromones.push_back( p );
}

void PheromoneMap::update( const float timeElapsed )
{
    auto it = m_vPheromones.begin();

    while( it != m_vPheromones.end() )
    {
        it->m_lifeTime += timeElapsed;
        if( it->m_lifeTime > it->m_maxLifeTime )
        {
            it = m_vPheromones.erase( it );
        }
        else
        {
            it++;
        }
    }

}

void PheromoneMap::draw( olc::PixelGameEngine& pge ) const
{
    olc::Pixel color;
    if( m_bHomePheromones )
    {
        color = olc::BLUE;
    }
    else
    {
        color = olc::RED;
    }


    for( const auto &p : m_vPheromones )
    {
        const float f = ( p.m_maxLifeTime - p.m_lifeTime ) / p.m_maxLifeTime;
        const int alpha = std::max( 20, int( f * 255 ) );
        auto transpColor = olc::Pixel( color.r, color.g, color.b, alpha );
        pge.FillCircle( p.m_pos, 1, transpColor );
    }
}

void PheromoneMap::reset()
{
    if( m_pMap )
    {
        delete[] m_pMap;
        m_pMap = nullptr;
    }

    m_pMap = new Pheromone[ ( int )m_screenWidth * ( int )m_screenHeight ];

    m_vPheromones.clear();
}
