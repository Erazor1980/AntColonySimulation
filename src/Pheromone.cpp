#include "Pheromone.h"
#define _USE_MATH_DEFINES 
#include <math.h>


void PheromoneMap::addPheromone( const olc::vf2d& pos, const bool bFoodPheromone )
{
    Pheromone p;
    p.m_pos = pos;
    p.m_lifeTime = 0.0f;

    if( bFoodPheromone )
    {
        p.m_color = olc::RED;
    }
    else
    {
        p.m_color = olc::BLUE;
    }

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

    for( const auto &p : m_vPheromones )
    {
        const float f = ( p.m_maxLifeTime - p.m_lifeTime ) / p.m_maxLifeTime;
        const int alpha = std::max( 20, int( f * 255 ) );
        auto transpColor = olc::Pixel( p.m_color.r, p.m_color.g, p.m_color.b, alpha );
        pge.FillCircle( p.m_pos, 1, transpColor );
    }
}

void PheromoneMap::reset()
{
    m_vPheromones.clear();
}
