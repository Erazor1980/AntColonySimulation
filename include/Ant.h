#pragma once
#include "olcPixelGameEngine.h"

class Ant
{
public:
    Ant( const olc::vf2d position, const float size );

    void draw( olc::PixelGameEngine& pge ) const;
    void update( const olc::PixelGameEngine& pge, const float timeElapsed );
    
private:
    struct Leg
    {
        olc::vf2d m_joints[ 4 ]; // 0 -> start (body connection)
    };

    void init( const olc::vf2d position, const float size );
    olc::vf2d transformPoint( const olc::vf2d& point ) const; // rotates and translates given point (based on m_pos and m_angle)
    void updateMotion();

    float       m_angle = 0.0f;
    olc::vf2d   m_pos;
    olc::vf2d   m_velocity = { 0, 0 };
    float       m_size;
    float       m_speed = 20;

    /* body, legs, antennas */
    std::pair< olc::vf2d, float > m_bodyParts[ 4 ]; // 0 = head, 1, 2, 3 ->remaining segments. pair < position, circle radius >
    std::vector< Leg > m_vLegs;
    olc::vf2d  m_leftAntenna[ 3 ];
    olc::vf2d m_rightAntenna[ 3 ];

    int m_animation = 0;                            // current animation state of the ant (movement)
    float m_timeNextMotion = 0.0f;                  // used to update moving animations
};