#pragma once
#include "olcPixelGameEngine.h"

class Ant
{
public:
    Ant( const olc::vf2d position, const float size );

    void draw( olc::PixelGameEngine& pge ) const;
    void update( const olc::PixelGameEngine& pge, const float timeElapsed );

    struct Leg
    {
        olc::vf2d m_joints[ 4 ]; // 0 -> start (body connection)
    };
private:
    void init( const olc::vf2d position, const float size );
    olc::vf2d transformPoint( const olc::vf2d& point ) const; // rotates and translates given point (based on m_pos and m_angle)

    float       m_angle = 0.0;
    olc::vf2d   m_pos;
    olc::vf2d   m_velocity = { 0, 0 };
    float       m_size;
    float       m_speed = 20;

    /* body, legs, antennas */
    std::pair< olc::vf2d, float >   m_bodyParts[ 4 ]; // 0 = head, 1, 2, 3 ->remaining segments. pair < position, circle radius >
    std::vector< Leg >              m_vLegs;
    olc::vf2d                       m_leftAntenna[ 3 ];
    olc::vf2d                       m_rightAntenna[ 3 ];
};