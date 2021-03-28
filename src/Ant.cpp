#include "..\include\Ant.h"

Ant::Ant( const olc::vf2d position, const int size )
{
    init( position, size );
}

void Ant::init( const olc::vf2d position, const int size )
{
    m_pos       = position;
    m_size      = size;
    m_angle     = ( float )rand() / ( float )RAND_MAX * 6.28318f;
        
    m_velocity.x = sinf( m_angle ) * m_speed;
    m_velocity.y = -cosf( m_angle ) * m_speed;
}
    
void Ant::draw( olc::PixelGameEngine& pge ) const
{
    pge.FillCircle( m_pos, m_size, olc::BLACK );
}

void Ant::update( const olc::PixelGameEngine& pge, const float timeElapsed )
{
    // VELOCITY changes POSITION (with respect to time)
    m_pos += m_velocity * timeElapsed;
}
