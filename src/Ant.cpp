#include "..\include\Ant.h"
#define _USE_MATH_DEFINES 
#include <math.h>

#define DEBUG_INFO 0

Ant::Ant( const olc::vf2d position, const float size, std::vector< olc::vf2d >& vFood, const olc::vf2d& nestPos, const float screenWidth, const float screenHeight )
    : 
    m_vFood( vFood ),
    m_nestPos( nestPos ),
    m_screenWidth( screenWidth ),
    m_screenHeight( screenHeight )
{
    init( position, size );
}

void Ant::init( const olc::vf2d position, const float size )
{
    m_pos           = position;
    m_size          = size;
    m_maxSpeed      = 100.0f + ( rand() % 50 );
    m_currSpeed     = m_maxSpeed * 0.7f;
    m_viewingAngle  = ( float )( 120 * M_PI / 180.0 );
    m_viewingRadius = size * 2.5f;
    m_lastPheromonePos = olc::vf2d( -1.0f, -1.0f );

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

    /* draw food (if picked up) */
    if( eStatus::_FOOD_COLLECTED == m_status || eStatus::_FOOD_FOUND == m_lastStatus )
    {
        const olc::vf2d pos = ( m_velocity.norm() * m_size / 4.0f ) + transformPoint( m_bodyParts[ 0 ].first );
        pge.FillCircle( pos, 4, olc::GREEN );
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

#if DEBUG_INFO
    const auto headPos = bodyPointsTransformed[ 0 ];
    const float angle = atan2f( m_velocity.y, m_velocity.x );
    const float leftAngle = angle - m_viewingAngle / 2.0f;
    const float rightAngle = angle + m_viewingAngle / 2.0f;

    olc::vf2d pntLeft;
    olc::vf2d pntRight;
    pntLeft.x   = cosf( leftAngle );
    pntLeft.y   = sinf( leftAngle );
    pntRight.x  = cosf( rightAngle );
    pntRight.y  = sinf( rightAngle );
    pntLeft     = ( pntLeft.norm() * m_viewingRadius ) + headPos;
    pntRight    = ( pntRight.norm() * m_viewingRadius ) + headPos;

    olc::vf2d foodPos;
    if( true == checkForFood( foodPos ) )
    {
        pge.DrawCircle( headPos, ( int )m_viewingRadius, olc::GREEN );
    }
    else
    {
        pge.DrawCircle( headPos, ( int )m_viewingRadius, olc::RED );
    }

    pge.DrawLine( headPos, pntLeft, olc::DARK_BLUE );
    pge.DrawLine( headPos, pntRight, olc::DARK_BLUE );

    pge.DrawLine( headPos, headPos + m_desiredDirection, olc::DARK_BLUE );

    pge.DrawStringDecal( m_pos - olc::vf2d( m_size, -m_size ), "Status: " + std::to_string( m_status ), olc::DARK_GREEN, { 1.5, 1.5 } );

    /* draw desired direction */
    auto desDirPnt = m_desiredDirection.norm() * 2 * m_size + transformPoint( m_bodyParts[ 0 ].first );
    pge.DrawLine( transformPoint( m_bodyParts[ 0 ].first ), desDirPnt, olc::WHITE );
#endif
}

void Ant::update( PheromoneMap* pHomePheromones, PheromoneMap* pFoodPheromones, const float timeElapsed )
{
    walk( timeElapsed );
    
    m_pos += m_velocity * timeElapsed;

    /* pheromone stuff */
    const auto pheromonePos = transformPoint( m_bodyParts[ 0 ].first );
    if( m_lastPheromonePos.x < 0 && m_lastPheromonePos.y < 0 ) // should only happen at the beginning, after leaving the nest for the first time
    {
        const float distToNest = ( m_pos - m_nestPos ).mag();
        if( distToNest > 5 )
        {
            m_lastPheromonePos = pheromonePos;
            pHomePheromones->addPheromone( m_lastPheromonePos );
        }
    }
    else
    {
        if( ( pheromonePos - m_lastPheromonePos ).mag() > m_distPheremones )
        {
            m_lastPheromonePos = pheromonePos;
            if( eStatus::_SEARCHING == m_status || eStatus::_FOOD_FOUND == m_status ||
                ( eStatus::_ROTATING == m_status && eStatus::_FOOD_COLLECTED == m_lastStatus ) )
            {
                pHomePheromones->addPheromone( m_lastPheromonePos );
            }
            else if( eStatus::_FOOD_COLLECTED == m_status || ( eStatus::_ROTATING == m_status && eStatus::_FOOD_FOUND == m_lastStatus ) )
            {
                pFoodPheromones->addPheromone( m_lastPheromonePos );
            }
        }
    }
    
    /* Animation */
    if( eStatus::_ROTATING == m_status )
    {
        m_timeNextMotion += timeElapsed * 100;
    }
    else
    {
        float test = m_velocity.mag();
        m_timeNextMotion += timeElapsed * m_velocity.mag();
    }
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

void Ant::setNestPos( const olc::vf2d & nestPos )
{
    m_nestPos = nestPos;
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

void Ant::walk( const float timeElapsed )
{
    float steerStrength = 1.5f;
    
    if( eStatus::_SEARCHING == m_status )
    {
        randomDirection();

        if( true == checkForFood( m_targetFoodPos ) )
        {
            m_desiredDirection = ( m_targetFoodPos - m_pos ).norm();
            m_status = eStatus::_FOOD_FOUND;
        }
    }
    else if( eStatus::_FOOD_FOUND == m_status )
    {
        const float pickupRadius = m_size / 4.0f;
        if( ( m_targetFoodPos - transformPoint( m_bodyParts[ 0 ].first ) ).mag() < pickupRadius )
        {
            if( true == pickUpFood() )
            {
                /* flip the desired direction and rotate by a bit (5°) */
                m_desiredDirection  = -m_desiredDirection;
                auto tmp            = m_desiredDirection;
                float angleDelta    = 5 * ( float )M_PI / 180.0f;;
                if( rand() % 2 )
                {
                    angleDelta *= -1;
                }                
                m_desiredDirection.x = tmp.x * cosf( angleDelta ) - tmp.y * sinf( angleDelta );
                m_desiredDirection.y = tmp.y * cosf( angleDelta ) + tmp.x * sinf( angleDelta );
                m_status            = eStatus::_ROTATING;
                m_lastStatus        = eStatus::_FOOD_FOUND;
            }
            else
            {
                m_status = eStatus::_SEARCHING;
            }
        }
        else
        {
            steerStrength       = 4.5f;
            m_desiredDirection  = ( m_targetFoodPos - m_pos ).norm();
            m_currSpeed         = m_maxSpeed * 0.65f;
        }
    }
    if( eStatus::_FOOD_COLLECTED == m_status )
    {
        /* brought food home */
        if( ( m_nestPos - transformPoint( m_bodyParts[ 0 ].first ) ).mag() < m_size / 2.0f )
        {
            /* flip the desired direction and rotate by a bit (5°) */
            m_desiredDirection  = -m_desiredDirection;
            auto tmp            = m_desiredDirection;
            float angleDelta    = 5 * ( float )M_PI / 180.0f;;
            if( rand() % 2 )
            {
                angleDelta *= -1;
            }
            m_desiredDirection.x = tmp.x * cosf( angleDelta ) - tmp.y * sinf( angleDelta );
            m_desiredDirection.y = tmp.y * cosf( angleDelta ) + tmp.x * sinf( angleDelta );
            m_status            = eStatus::_ROTATING;
            m_lastStatus        = eStatus::_FOOD_COLLECTED;
        }
        else
        {
            steerStrength       = 3.5f;
            m_desiredDirection  = ( m_nestPos - m_pos ).norm();
            m_currSpeed         = m_maxSpeed * 0.75f;
        }
    }

    if( eStatus::_ROTATING == m_status )
    {
        const float currAngle       = atan2f( m_velocity.y, m_velocity.x );
        const float desiredAngle    = atan2f( m_desiredDirection.y, m_desiredDirection.x );

        if( fabsf( currAngle - desiredAngle ) <= 5 * M_PI / 180.0f )
        {
            if( m_lastStatus == eStatus::_FOOD_FOUND )
            {
                m_status = eStatus::_FOOD_COLLECTED;
            }
            else
            {
                m_status = eStatus::_SEARCHING;
            }            
        }
        else
        {
            m_currSpeed = 0.1f * m_maxSpeed;
            steerStrength = 15.0f;
        }
    }

    /* boundary checks */
    {
        const auto headPos = transformPoint( m_bodyParts[ 0 ].first );
        if( headPos.x < m_size && m_velocity.x < 0 )
        {
            m_desiredDirection.x = 100;
            steerStrength = 5.0f;
        }
        else if( headPos.x > m_screenWidth - m_size && m_velocity.x > 0 )
        {
            m_desiredDirection.x = -100;
            steerStrength = 5.0f;
        }
        if( headPos.y < m_size && m_velocity.y < 0 )
        {
            m_desiredDirection.y = 100;
            steerStrength = 5.0f;
        }
        else if( headPos.y > m_screenHeight - m_size && m_velocity.y > 0 )
        {
            m_desiredDirection.y = -100;
            steerStrength = 5.0f;
        }
    }

    olc::vf2d desiredVelocity = m_desiredDirection * m_currSpeed;
    olc::vf2d desiredSteeringForce = ( desiredVelocity - m_velocity ) * steerStrength;
    olc::vf2d acceleration = desiredSteeringForce;
    if( acceleration.mag() > steerStrength )
    {
        acceleration = acceleration.norm() * steerStrength;
    }
    m_velocity = m_velocity + acceleration * timeElapsed * 50;
    if( m_velocity.mag() > m_currSpeed )
    {
        m_velocity = m_velocity.norm() * m_currSpeed;
    }
    if( m_velocity.mag() < 45 )
    {
        m_velocity = m_velocity.norm() * 45;
    }


}

void Ant::randomDirection()
{
    const float wanderStrength = 0.17f;
    const float rndAngle = ( float )rand() / ( float )RAND_MAX * 6.28318f;
    olc::vf2d rndPntUnitCircle;
    rndPntUnitCircle.x = sinf( rndAngle );
    rndPntUnitCircle.y = -cosf( rndAngle );
    m_desiredDirection = ( m_desiredDirection + rndPntUnitCircle * wanderStrength ).norm();
    m_currSpeed = m_maxSpeed * 0.7f;
}

bool Ant::scanForPheromones( PheromoneMap * pHomePheromones, PheromoneMap * pFoodPheromones )
{
    return false;
}

bool Ant::checkForFood( olc::vf2d& foodPos ) const
{
    if( m_vFood.empty() )
    {
        return false;
    }

    const auto headPos = transformPoint( m_bodyParts[ 0 ].first );
    const float angle = atan2f( m_velocity.y, m_velocity.x );
    const float leftAngle = angle - m_viewingAngle / 2.0f;
    const float rightAngle = angle + m_viewingAngle / 2.0f;

    bool bFoodFound = false;
    for( auto f : m_vFood )
    {
        if( ( headPos - f ).mag() < m_viewingRadius )
        {
            const float footAngle = atan2f( ( f - headPos ).y, ( f - headPos ).x );
            if( footAngle > leftAngle && footAngle < rightAngle )
            {
                bFoodFound = true;
                foodPos = f;
                break;
            }
        }
    }

    return bFoodFound;
}

bool Ant::pickUpFood()
{
    for( int i = 0; i < ( int )m_vFood.size(); ++i )
    {
        if( m_vFood[ i ] == m_targetFoodPos )
        {
            m_vFood.erase( m_vFood.begin() + i );
            return true;
        }
    }
    return false;
}
