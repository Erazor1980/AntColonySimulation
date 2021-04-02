#pragma once
#include "Ant.h"

class AntColonyOptimization : public olc::PixelGameEngine
{
public:
    AntColonyOptimization();
    ~AntColonyOptimization();
public:
    bool OnUserCreate() override;
    bool OnUserUpdate( float timeElapsed ) override;

private:
    void composeFrame();
    void reset( const bool bOnlyOneAnt = false );
    
    std::vector< Ant > m_vAnts;

    std::vector< olc::vf2d > m_vFood;

    olc::vf2d m_nestPos;

    PheromoneMap* m_pHomePheromones = nullptr;
    PheromoneMap* m_pFoodPheromones = nullptr;

    const float m_updateIntervalPheromones = 0.5f;  // in seconds
    float m_timeSinceLastPheromonesUpdate = 0.0f;

    bool m_bDrawPheromones = true;
};