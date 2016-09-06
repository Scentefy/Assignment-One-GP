#ifndef VECTOR_H
#define VECTOR_H

class Vector2D
{
	float m_x;
	float m_y;
public:
	Vector2D(float x, float y);
	float getLength();
	static float dot(Vector2D a,Vector2D b);
	//static Vector2D proj(…);
};
#endif