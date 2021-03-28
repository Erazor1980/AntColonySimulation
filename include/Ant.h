#pragma once
#include "olcPixelGameEngine.h"

class Ant
{
public:
    Ant( const olc::vf2d position, const int size );

    void draw( olc::PixelGameEngine& pge ) const;
    void update( const olc::PixelGameEngine& pge, const float timeElapsed );

private:
    void init( const olc::vf2d position, const int size );

    float       m_angle = 0.0;
    olc::vf2d   m_pos;
    olc::vf2d   m_velocity = { 0, 0 };
    int         m_size;
    int         m_speed = 20;
};