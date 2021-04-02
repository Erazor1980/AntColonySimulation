#pragma once
#include "olcPixelGameEngine.h"

class PheromoneMap
{
public:
    PheromoneMap( const int screenWidth, const int screenHeigth, const bool bHomePheromone = true /* if true -> home pheromone, if false -> food pheromone */ );
    ~PheromoneMap();

    void addPheromone( const olc::vf2d& pos );
    void update( const float timeElapsed );
    void draw( olc::PixelGameEngine& pge ) const;
    void reset();
private:
    struct Pheromone
    {
        float m_maxLifeTime = 10.0; /* in seconds */
        float m_lifeTime = 0.0f;
    };

    olc::vf2d getPosFromIdx( const int idx ) const;
    int getIdxFromPos( const int x, const int y ) const;
    int getIdxFromPos( const olc::vf2d& pnt ) const;

    Pheromone* m_pMap = nullptr;
    int m_screenWidth;
    int m_screenHeight;

    bool m_bHomePheromones; /* if true -> home pheromone (blue), if false -> food pheromone (red) */

    std::vector< int > m_vActivePheromonesIndices;
};
