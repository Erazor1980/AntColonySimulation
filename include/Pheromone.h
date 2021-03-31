#pragma once
#include "olcPixelGameEngine.h"

class PheromoneMap
{
public:
    PheromoneMap() {}

    void addPheromone( const olc::vf2d& pos, const bool bFoodPheromone = false /* if true -> house pheromone */ );
    void update( const float timeElapsed );
    void draw( olc::PixelGameEngine& pge ) const;
    void reset();
private:
    struct Pheromone
    {
        float m_maxLifeTime = 10.0; /* in seconds */
        float m_lifeTime = 0.0f;

        olc::vf2d m_pos;
        olc::Pixel m_color; /* red -> food pheromone, blue -> home */
    };

    std::vector< Pheromone > m_vPheromones;
};
