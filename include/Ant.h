#pragma once
#include "olcPixelGameEngine.h"

class Ant
{
public:
    Ant( const olc::vf2d position, const float size, std::vector< olc::vf2d >& vFood, const olc::vf2d& nestPos );

    void draw( olc::PixelGameEngine& pge ) const;
    void update( const olc::PixelGameEngine& pge, const float timeElapsed );
    
    void setNestPos( const olc::vf2d& nestPos );    /* to be able to change it ouside */
private:
    struct Leg
    {
        olc::vf2d m_joints[ 4 ]; // 0 -> start (body connection)
    };

    enum eStatus
    {
        _SEARCHING = 0,     /* searching for food */
        _FOOD_FOUND,        /* spotted food, going in its diraction */
        _FOOD_COLLECTED     /* food collected, on the way back home */
    };

    void init( const olc::vf2d position, const float size );
    olc::vf2d transformPoint( const olc::vf2d& point ) const; // rotates and translates given point (based on m_pos and m_angle)
    void updateMotion();
    void walk( const float timeElapsed );  // running in random direction, searching for food
    bool checkForFood( olc::vf2d& foodPos ) const;
    void pickUpFood();

    olc::vf2d   m_pos;
    olc::vf2d   m_velocity = { 0, 0 };
    float       m_size;
    float       m_maxSpeed;
    olc::vf2d   m_desiredDirection;
    float       m_viewingAngle;
    float       m_viewingRadius;

    olc::vf2d   m_targetFoodPos;
    std::vector< olc::vf2d >& m_vFood;
    olc::vf2d m_nestPos;

    eStatus m_status = _SEARCHING;

    /* body, legs, antennas */
    std::pair< olc::vf2d, float > m_bodyParts[ 4 ]; // 0 = head, 1, 2, 3 ->remaining segments. pair < position, circle radius >
    std::vector< Leg > m_vLegs;
    olc::vf2d  m_leftAntenna[ 3 ];
    olc::vf2d m_rightAntenna[ 3 ];

    int m_animation = 0;                            // current animation state of the ant (movement)
    float m_timeNextMotion = 0.0f;                  // used to update moving animations
};