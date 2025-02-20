
#include "Quadrilateral.h"
#include "Ray2d.h"

Quadrilateral::Quadrilateral ( const Vector2f& a, const Vector2f& b, const Vector2f& c, const Vector2f& d )
{
	m_corners[0] = a;
	m_corners[1] = b;
	m_corners[2] = c;
	m_corners[3] = d;
}
Quadrilateral::Quadrilateral ( void )
{
	m_corners[0] = Vector2f(0,0);
	m_corners[1] = Vector2f(0,0);
	m_corners[2] = Vector2f(0,0);
	m_corners[3] = Vector2f(0,0);
}


Real Quadrilateral::Area ( void ) const
{
	Real area;
	area = 0.5f * (m_corners[2]-m_corners[0]).cross(m_corners[3]-m_corners[1]);
	return fabs(area);
}

Vector2f&		Quadrilateral::operator[] ( const int index )
{
	if ( index < 0 ) {
		return m_corners[0];
	}
	else if ( index < 4 ) {
		return m_corners[index];
	}
	else {
		return m_corners[3];
	}
}
const Vector2f& Quadrilateral::operator[] ( const int index ) const
{
	if ( index < 0 ) {
		return m_corners[0];
	}
	else if ( index < 4 ) {
		return m_corners[index];
	}
	else {
		return m_corners[3];
	}
}


int Quadrilateral::Slice ( const Vector2f& ray_pos, const Vector2f& ray_dir, Quadrilateral& out_quad1, Quadrilateral& out_quad2 ) const
{
	return 1;
}