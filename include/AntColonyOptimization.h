#pragma once
#include "Ant.h"

class AntColonyOptimization : public olc::PixelGameEngine
{
public:
    AntColonyOptimization();

public:
    bool OnUserCreate() override;
    bool OnUserUpdate( float timeElapsed ) override;

private:
    void composeFrame();
    void reset( const bool bOnlyOneAnt = false );
    
    std::vector< Ant > m_vAnts;

    std::vector< olc::vf2d > m_vFood;

    olc::vf2d m_nestPos;
};