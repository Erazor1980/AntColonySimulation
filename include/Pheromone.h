#pragma once
#include "olcPixelGameEngine.h"

class PheromoneMap
{
public:
    PheromoneMap() {}
    PheromoneMap( const int screenWidth,
                  const int screenHeight,
                  const bool bFoodPheromone, /* if false -> home pheromone*/
                  const int gridSize = 5 /* in pixel, has to be odd */ );

    ~PheromoneMap();

    void draw( olc::PixelGameEngine& pge ) const;
private:
    struct Pheromone
    {
        const float m_maxLifeTime = 10.0; /* in seconds */
        float m_lifeTime = 0.0f;

    };

    Pheromone* mp_map;

    int m_screenWidth;
    int m_screenHeight;
    int m_gridSize;

    olc::Pixel m_color; /* red -> food pheromone, blue -> home */
};
