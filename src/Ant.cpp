#include "..\include\Ant.h"
#define _USE_MATH_DEFINES 
#include <math.h>

Ant::Ant( const olc::vf2d position, const float size )
{
    init( position, size );
}

void Ant::init( const olc::vf2d position, const float size )
{
    m_pos       = position;
    m_size      = size;
    //m_maxSpeed  = 25.0f + rand() % 50;
    m_maxSpeed  = 75;
    
    const float rndAngle = ( float )rand() / ( float )RAND_MAX * 6.28318f;

    m_velocity.x = sinf( rndAngle ) * 25;
    m_velocity.y = -cosf( rndAngle ) * 25;


    /* default body positions (head top) */
    m_bodyParts[ 0 ].first = { 0, -0.35f * m_size };   /* head */
    m_bodyParts[ 0 ].second = 0.17f * m_size;
    m_bodyParts[ 1 ].first = { 0, -0.125f * m_size };
    m_bodyParts[ 1 ].second = 0.11f * m_size;
    m_bodyParts[ 2 ].first = { 0, 0 };
    m_bodyParts[ 2 ].second = 0.08f * m_size;
    m_bodyParts[ 3 ].first = { 0, 0.27f * m_size };   /* abdomen */
    m_bodyParts[ 3 ].second = 0.205f * m_size;

    /* legs */
    m_vLegs.push_back( { m_bodyParts[ 1 ].first, { -m_size / 4, -m_size / 5 }, { -m_size / 2.5f, -m_size * 0.375f }, { -m_size / 2.2f, -m_size * 0.35f } } );   // top left
    m_vLegs.push_back( { m_bodyParts[ 1 ].first, { m_size / 4, -m_size / 5 }, { m_size / 2.5f, -m_size * 0.375f }, { m_size / 2.2f, -m_size * 0.35f } } );      // top right
    m_vLegs.push_back( { m_bodyParts[ 1 ].first + olc::vf2d( 0, 0.1f * m_size ), { -m_size / 3.5f, m_bodyParts[ 1 ].first.y },
                       { -m_size / 2.3f, m_size * 0.075f }, { -m_size / 2.1f, m_size * 0.075f } } );    // middle left
    m_vLegs.push_back( { m_bodyParts[ 1 ].first + olc::vf2d( 0, 0.1f * m_size ), { m_size / 3.5f, m_bodyParts[ 1 ].first.y },
                       { m_size / 2.3f, m_size * 0.075f }, { m_size / 2.1f, m_size * 0.075f } } );      // middle right
    m_vLegs.push_back( { m_bodyParts[ 1 ].first + olc::vf2d( 0, 0.1f * m_size ),{ -m_size / 3.5f, m_size * 0.1f },
                       { -m_size / 3.3f, m_size * 0.5f },{ -m_size / 2.8f, m_size * 0.55f } } );        // bottom left
    m_vLegs.push_back( { m_bodyParts[ 1 ].first + olc::vf2d( 0, 0.1f * m_size ),{ m_size / 3.5f, m_size * 0.1f },
                       { m_size / 3.3f, m_size * 0.5f },{ m_size / 2.8f, m_size * 0.55f } } );        // bottom right
    
    /* antennas */
    m_leftAntenna[ 0 ] = m_bodyParts[ 0 ].first - olc::vf2d( 0, 0.05f * m_size );
    m_leftAntenna[ 1 ] = m_bodyParts[ 0 ].first - olc::vf2d( 0.25f * m_size, 0.20f * m_size );
    m_leftAntenna[ 2 ] = m_bodyParts[ 0 ].first - olc::vf2d( 0.20f * m_size, 0.40f * m_size );

    m_rightAntenna[ 0 ] = m_bodyParts[ 0 ].first - olc::vf2d( 0, 0.05f * m_size );
    m_rightAntenna[ 1 ] = m_bodyParts[ 0 ].first - olc::vf2d( -0.25f * m_size, 0.20f * m_size );
    m_rightAntenna[ 2 ] = m_bodyParts[ 0 ].first - olc::vf2d( -0.20f * m_size, 0.40f * m_size );
}
   
void Ant::draw( olc::PixelGameEngine& pge ) const
{
    /* transform all points */
    olc::vf2d bodyPointsTransformed[ 4 ];
    std::vector< Leg > vLegPointsTransformed;
    olc::vf2d leftAntennaTransformed[ 3 ];
    olc::vf2d rightAntennaTransformed[ 3 ];

    for( int i = 0; i < 4; ++i )
    {
        bodyPointsTransformed[ i ] = transformPoint( m_bodyParts[ i ].first );
    }

    for( const auto& l : m_vLegs )
    {
        Leg transformedLeg;
        for( int i = 0; i < 4; ++i )
        {
            transformedLeg.m_joints[ i ] = transformPoint( l.m_joints[ i ] );
        }
        vLegPointsTransformed.push_back( transformedLeg );
    }

    for( int i = 0; i < 3; ++i )
    {
        leftAntennaTransformed[ i ] = transformPoint( m_leftAntenna[ i ] );
        rightAntennaTransformed[ i ] = transformPoint( m_rightAntenna[ i ] );
    }


    /* draw legs */
    for( const auto& l : vLegPointsTransformed )
    {
        for( int i = 1; i < 4; ++i )
        {
            pge.DrawLine( l.m_joints[ i ], l.m_joints[ i - 1 ], olc::Pixel( 101, 67, 33 ) );
        }
    }

    /* draw antennas */
    for( int i = 0; i < 3; ++i )
    {
        for( int i = 1; i < 3; ++i )
        {
            pge.DrawLine( leftAntennaTransformed[ i ], leftAntennaTransformed[ i - 1 ], olc::Pixel( 101, 67, 33 ) );
            pge.DrawLine( rightAntennaTransformed[ i ], rightAntennaTransformed[ i - 1 ], olc::Pixel( 101, 67, 33 ) );
        }
    }

    /* draw body */
    for( int i = 0; i < 4; ++i )
    {
        pge.FillCircle( bodyPointsTransformed[ i ], ( int )m_bodyParts[ i ].second, olc::BLACK );
    }
}

void Ant::update( const olc::PixelGameEngine& pge, const float timeElapsed )
{
    if( eStatus::_SEARCHING == m_status )
    {
        searching( timeElapsed );
    }
    

    m_pos += m_velocity * timeElapsed;
    
    /* Animation */
    m_timeNextMotion += timeElapsed * m_velocity.mag();
    if( m_timeNextMotion > 5 )
    {
        m_timeNextMotion = 0.0f;
        m_animation++;
        if( m_animation > 3 )
        {
            m_animation = 0;
        }
        updateMotion();
    }
}

olc::vf2d Ant::transformPoint( const olc::vf2d& point ) const
{
    olc::vf2d transfromedPoint;

    const float angle = atan2f( m_velocity.y, m_velocity.x ) + ( float )M_PI_2;

    /* rotate first */
    transfromedPoint.x = point.x * cosf( angle ) - point.y * sinf( angle );
    transfromedPoint.y = point.y * cosf( angle ) + point.x * sinf( angle );

    /* translate */
    transfromedPoint += m_pos;

    return transfromedPoint;
}

void Ant::updateMotion()
{
    m_vLegs.clear();
    m_vLegs.push_back( { m_bodyParts[ 1 ].first,{ -m_size / 4, -m_size / 5 },{ -m_size / 2.5f, -m_size * 0.375f },{ -m_size / 2.2f, -m_size * 0.35f } } );   // top left
    m_vLegs.push_back( { m_bodyParts[ 1 ].first,{ m_size / 4, -m_size / 5 },{ m_size / 2.5f, -m_size * 0.375f },{ m_size / 2.2f, -m_size * 0.35f } } );      // top right
    m_vLegs.push_back( { m_bodyParts[ 1 ].first + olc::vf2d( 0, 0.1f * m_size ),{ -m_size / 3.5f, m_bodyParts[ 1 ].first.y },
                       { -m_size / 2.3f, m_size * 0.075f },{ -m_size / 2.1f, m_size * 0.075f } } );    // middle left
    m_vLegs.push_back( { m_bodyParts[ 1 ].first + olc::vf2d( 0, 0.1f * m_size ),{ m_size / 3.5f, m_bodyParts[ 1 ].first.y },
                       { m_size / 2.3f, m_size * 0.075f },{ m_size / 2.1f, m_size * 0.075f } } );      // middle right
    m_vLegs.push_back( { m_bodyParts[ 1 ].first + olc::vf2d( 0, 0.1f * m_size ),{ -m_size / 3.5f, m_size * 0.1f },
                       { -m_size / 3.3f, m_size * 0.5f },{ -m_size / 2.8f, m_size * 0.55f } } );        // bottom left
    m_vLegs.push_back( { m_bodyParts[ 1 ].first + olc::vf2d( 0, 0.1f * m_size ),{ m_size / 3.5f, m_size * 0.1f },
                       { m_size / 3.3f, m_size * 0.5f },{ m_size / 2.8f, m_size * 0.55f } } );        // bottom right
    if( 0 == m_animation )
    {
        
    }
    else if( 1 == m_animation )
    {
        m_vLegs[ 0 ].m_joints[ 1 ].y -= m_size * 0.02f;
        m_vLegs[ 0 ].m_joints[ 2 ].x += m_size * 0.05f;
        m_vLegs[ 0 ].m_joints[ 2 ].y -= m_size * 0.02f;
        m_vLegs[ 0 ].m_joints[ 3 ].y -= m_size * 0.1f;
        m_vLegs[ 0 ].m_joints[ 3 ].x += m_size * 0.02f;

        m_vLegs[ 1 ].m_joints[ 1 ].y += m_size * 0.02f;
        m_vLegs[ 1 ].m_joints[ 2 ].x += m_size * 0.05f;
        m_vLegs[ 1 ].m_joints[ 2 ].y += m_size * 0.02f;
        m_vLegs[ 1 ].m_joints[ 3 ].y += m_size * 0.05f;
        m_vLegs[ 1 ].m_joints[ 3 ].x += m_size * 0.02f;

        m_vLegs[ 2 ].m_joints[ 1 ].y += m_size * 0.08f;
        m_vLegs[ 2 ].m_joints[ 2 ].y += m_size * 0.08f;
        m_vLegs[ 2 ].m_joints[ 3 ].y += m_size * 0.08f;

        m_vLegs[ 3 ].m_joints[ 1 ].y -= m_size * 0.02f;
        m_vLegs[ 3 ].m_joints[ 2 ].y -= m_size * 0.05f;
        m_vLegs[ 3 ].m_joints[ 3 ].y -= m_size * 0.05f;

        m_vLegs[ 4 ].m_joints[ 1 ].y -= m_size * 0.04f;
        m_vLegs[ 4 ].m_joints[ 2 ].y -= m_size * 0.04f;
        m_vLegs[ 4 ].m_joints[ 3 ].y -= m_size * 0.04f;
        m_vLegs[ 4 ].m_joints[ 3 ].x -= m_size * 0.02f;

        m_vLegs[ 5 ].m_joints[ 1 ].y += m_size * 0.07f;
        m_vLegs[ 5 ].m_joints[ 2 ].y += m_size * 0.07f;
        m_vLegs[ 5 ].m_joints[ 3 ].y += m_size * 0.07f;
        m_vLegs[ 5 ].m_joints[ 3 ].x += m_size * 0.02f;
    }
    else if( 3 == m_animation )
    {
        m_vLegs[ 0 ].m_joints[ 1 ].y += m_size * 0.02f;
        m_vLegs[ 0 ].m_joints[ 2 ].x -= m_size * 0.05f;
        m_vLegs[ 0 ].m_joints[ 2 ].y += m_size * 0.02f;
        m_vLegs[ 0 ].m_joints[ 3 ].y += m_size * 0.05f;
        m_vLegs[ 0 ].m_joints[ 3 ].x -= m_size * 0.02f;

        m_vLegs[ 1 ].m_joints[ 1 ].y -= m_size * 0.02f;
        m_vLegs[ 1 ].m_joints[ 2 ].x -= m_size * 0.05f;
        m_vLegs[ 1 ].m_joints[ 2 ].y -= m_size * 0.02f;
        m_vLegs[ 1 ].m_joints[ 3 ].y -= m_size * 0.1f;
        m_vLegs[ 1 ].m_joints[ 3 ].x -= m_size * 0.02f;

        m_vLegs[ 2 ].m_joints[ 1 ].y -= m_size * 0.02f;
        m_vLegs[ 2 ].m_joints[ 2 ].y -= m_size * 0.05f;
        m_vLegs[ 2 ].m_joints[ 3 ].y -= m_size * 0.05f;

        m_vLegs[ 3 ].m_joints[ 1 ].y += m_size * 0.08f;
        m_vLegs[ 3 ].m_joints[ 2 ].y += m_size * 0.08f;
        m_vLegs[ 3 ].m_joints[ 3 ].y += m_size * 0.08f;

        m_vLegs[ 4 ].m_joints[ 1 ].y += m_size * 0.07f;
        m_vLegs[ 4 ].m_joints[ 2 ].y += m_size * 0.07f;
        m_vLegs[ 4 ].m_joints[ 3 ].y += m_size * 0.07f;
        m_vLegs[ 4 ].m_joints[ 3 ].x -= m_size * 0.02f;        

        m_vLegs[ 5 ].m_joints[ 1 ].y -= m_size * 0.04f;
        m_vLegs[ 5 ].m_joints[ 2 ].y -= m_size * 0.04f;
        m_vLegs[ 5 ].m_joints[ 3 ].y -= m_size * 0.04f;
        m_vLegs[ 5 ].m_joints[ 3 ].x += m_size * 0.02f;
    }
}

void Ant::searching( const float timeElapsed )
{
    const float wanderStrength = 0.07f;
    const float steerStrength = 2;
    const float rndAngle = ( float )rand() / ( float )RAND_MAX * 6.28318f;
    olc::vf2d rndPntUnitCircle;
    rndPntUnitCircle.x = sinf( rndAngle );
    rndPntUnitCircle.y = -cosf( rndAngle );

    m_desiredDirection = ( m_desiredDirection + rndPntUnitCircle * wanderStrength ).norm();

    olc::vf2d desiredVelocity = m_desiredDirection * m_maxSpeed;
    olc::vf2d desiredSteeringForce = ( desiredVelocity - m_velocity ) * steerStrength;
    olc::vf2d acceleration = desiredSteeringForce;
    if( acceleration.mag() > steerStrength )
    {
        acceleration = acceleration.norm() * steerStrength;
    }
    m_velocity = m_velocity + acceleration * timeElapsed * 50;
    if( m_velocity.mag() > m_maxSpeed )
    {
        m_velocity = m_velocity.norm() * m_maxSpeed;
    }
}
