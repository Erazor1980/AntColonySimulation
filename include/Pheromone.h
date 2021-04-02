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

        olc::vf2d m_pos;
    };

    Pheromone* m_pMap = nullptr;
    float m_screenWidth;
    float m_screenHeight;

    bool m_bHomePheromones; /* if true -> home pheromone (blue), if false -> food pheromone (red) */

    std::vector< Pheromone > m_vPheromones;
};
