#include "..\include\Ant.h"

Ant::Ant( const olc::vf2d position, const float size )
{
    init( position, size );
}

void Ant::init( const olc::vf2d position, const float size )
{
    m_pos       = position;
    m_size      = size;
    m_angle     = ( float )rand() / ( float )RAND_MAX * 6.28318f;

    m_speed = 5 + rand() % 100;
        
    m_velocity.x = sinf( m_angle ) * m_speed;
    m_velocity.y = -cosf( m_angle ) * m_speed;

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
    // VELOCITY changes POSITION (with respect to time)
    m_pos += m_velocity * timeElapsed;
}

olc::vf2d Ant::transformPoint( const olc::vf2d& point ) const
{
    olc::vf2d transfromedPoint;

    /* rotate first */
    transfromedPoint.x = point.x * cosf( m_angle ) - point.y * sinf( m_angle );
    transfromedPoint.y = point.y * cosf( m_angle ) + point.x * sinf( m_angle );

    /* translate */
    transfromedPoint += m_pos;

    return transfromedPoint;
}