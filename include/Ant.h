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

    olc::vf2d   m_pos;
    int         m_size;
};