#include "Pheromone.h"
#define _USE_MATH_DEFINES 
#include <math.h>


PheromoneMap::PheromoneMap( const int screenWidth, const int screenHeigth, const bool bHomePheromone )
{
    m_screenWidth       = screenWidth;
    m_screenHeight      = screenHeigth;
    m_bHomePheromones   = bHomePheromone;
    reset();
}

PheromoneMap::~PheromoneMap()
{
    if( m_pMap )
    {
        delete[] m_pMap;
        m_pMap = nullptr;
    }
}

void PheromoneMap::addPheromone( const olc::vf2d& pos )
{
    /* check for position outside screen */
    if( pos.x < 0 || pos.x >= m_screenWidth || pos.y < 0 || pos.y >= m_screenHeight )
    {
        return;
    }

    const int idx = getIdxFromPos( pos );

    /* check if idx is already added */
    if( std::find( m_vActivePheromonesIndices.begin(), m_vActivePheromonesIndices.end(), idx ) != m_vActivePheromonesIndices.end() )
    {
        m_pMap[ idx ].m_lifeTime = 0.0f;
        return;
    }

    Pheromone p;
    p.m_lifeTime = 0.0f;
    p.m_bActive = true;
    
    m_pMap[ idx ] = p;
    m_vActivePheromonesIndices.push_back( idx );
}

void PheromoneMap::update( const float timeElapsed )
{
    auto it = m_vActivePheromonesIndices.begin();

    while( it != m_vActivePheromonesIndices.end() )
    {
        Pheromone& p = m_pMap[ *it ];
        p.m_lifeTime += timeElapsed;
        if( p.m_lifeTime > p.m_maxLifeTime )
        {
            p.m_bActive = false;
            it = m_vActivePheromonesIndices.erase( it );
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


    for( const auto &idx : m_vActivePheromonesIndices )
    {
        Pheromone& p = m_pMap[ idx ];
        const float f = ( p.m_maxLifeTime - p.m_lifeTime ) / p.m_maxLifeTime;
        const int alpha = std::max( 20, int( f * 255 ) );
        auto transpColor = olc::Pixel( color.r, color.g, color.b, alpha );
        pge.FillCircle( getPosFromIdx( idx ), 1, transpColor );
    }
}

void PheromoneMap::reset()
{
    if( m_pMap )
    {
        delete[] m_pMap;
        m_pMap = nullptr;
    }

    m_pMap = new Pheromone[ m_screenWidth * m_screenHeight ];

    m_vActivePheromonesIndices.clear();
}

olc::vf2d PheromoneMap::getPosFromIdx( const int idx ) const
{
    return olc::vf2d( ( float )( idx % m_screenWidth ), ( float )( idx / m_screenWidth ) );
}

int PheromoneMap::getIdxFromPos( const int x, const int y ) const
{
    return y * m_screenWidth + x;
}

int PheromoneMap::getIdxFromPos( const olc::vf2d& pnt ) const
{
    return getIdxFromPos( ( int )pnt.x, ( int )pnt.y );
}

float PheromoneMap::getPheromonesValue( const olc::vf2d& pos, const float radius )
{
    const float radSqrd = radius * radius;
    float value = 0.0f;

    /* go through all pixels within the circle at 'pos' with radius 'radius' */
    /* using bounding rectangle and distance 'pos' to current pixel (x,y) */
    for( int x = ( int )( pos.x - radius ); x <= ( int )( pos.x + radius ); ++x )
    {
        for( int y = ( int )( pos.y - radius ); y <= ( int )( pos.y + radius ); ++y )
        {
            /* check for position outside screen */
            if( x < 0 || x >= m_screenWidth || y < 0 || y >= m_screenHeight )
            {
                continue;
            }
            if( ( olc::vf2d( x, y ) - pos ).mag2() <= radSqrd ) /* we are within the circle */
            {
                const auto p = m_pMap[ getIdxFromPos( x, y ) ];
                if( p.m_bActive )
                {
                    value += ( p.m_maxLifeTime - p.m_lifeTime );
                }
            }
        }
    }

    return value;
}
