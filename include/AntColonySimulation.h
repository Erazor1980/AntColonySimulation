#pragma once
#include "Ant.h"

class AntColonySimulation : public olc::PixelGameEngine
{
public:
    AntColonySimulation();
    ~AntColonySimulation();
public:
    bool OnUserCreate() override;
    bool OnUserUpdate( float timeElapsed ) override;

private:
    void composeFrame();
    void reset( const int numberOfAnts );
    
    std::vector< Ant > m_vAnts;

    std::vector< olc::vf2d > m_vFood;

    olc::vf2d m_nestPos;

    PheromoneMap* m_pHomePheromones = nullptr;
    PheromoneMap* m_pFoodPheromones = nullptr;

    const float m_updateIntervalPheromones = 0.5f;  // in seconds
    float m_timeSinceLastPheromonesUpdate = 0.0f;

    bool m_bDrawPheromones = true;
    bool m_bDrawDebugStuff = false;
};