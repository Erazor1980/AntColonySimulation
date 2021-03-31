#include "Pheromone.h"
#define _USE_MATH_DEFINES 
#include <math.h>


PheromoneMap::PheromoneMap( const int screenWidth, const int screenHeight, const bool bFoodPheromone, const int gridSize )
{
    m_screenWidth   = screenWidth;
    m_screenHeight  = screenHeight;
    m_gridSize      = gridSize;
    if( m_gridSize < 5 )
    {
        m_gridSize = 5;
    }
    if( m_gridSize % 2 == 0 )
    {
        m_gridSize++;
    }

    if( bFoodPheromone )
    {
        m_color = olc::RED;
    }
    else
    {
        m_color = olc::BLUE;
    }

    mp_map = new Pheromone[ m_screenWidth / m_gridSize * m_screenHeight / m_gridSize ];
}

PheromoneMap::~PheromoneMap()
{
    if( mp_map )
    {
        delete[] mp_map;
        mp_map = nullptr;
    }
}

void PheromoneMap::draw( olc::PixelGameEngine& pge ) const
{
    const int startPx = m_gridSize / 2; /* since grid is quadratic, it applies for x and y direction */
    for( int y = startPx; y < m_screenHeight; y += m_gridSize )
    {
        const int rowOffset = y * m_screenWidth;
        for( int x = startPx; x < m_screenWidth; x += m_gridSize )
        {
            pge.FillCircle( olc::vi2d( x, y ), 2, m_color );
        }
    }
}
