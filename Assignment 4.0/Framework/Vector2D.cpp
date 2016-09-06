#include <cmath>
#include "Vector2D.h"
using namespace std;

Vector2D::Vector2D(float x, float y)
{
	m_x = x;
	m_y = y;
}

float Vector2D::getLength()
{
	return sqrt(m_x*m_x + m_y*m_y);
}

float 
Vector2D::dot(Vector2D a, Vector2D b)
{
	return ((a.m_x * b.m_x) + (a.m_y * b.m_y));
}