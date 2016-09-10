// COMP710 GP 2D Framework
#ifndef __ENTITY_H__
#define __ENTITY_H__
#include "backbuffer.h"

// Forward Declarations:
class BackBuffer;
class Sprite;

class Entity
{
	//Member Methods:
public:
	Entity();
	~Entity();

	bool Initialise(Sprite* sprite);
	int GetSpriteWidth();

	void Process(float deltaTime);
	void Draw(BackBuffer& backBuffer);

	void SetDead(bool dead);
	bool IsDead() const;

	char GetMask();
	void SetMask(char m);

	bool IsCollidingWith(Entity& e);

	float GetPositionX();
	float GetPositionY();

	void SetPositionX(float x);
	void SetPositionY(float y);

	float GetHorizontalVelocity();
	void SetHorizontalVelocity(float x); 

	float GetVerticalVelocity();
	void SetVerticalVelocity(float y);

	char GetTileType();
	void SetCharType(char t);

protected:

private:
	Entity(const Entity& entity);
	Entity& operator=(const Entity& entity);

	//Member Data:
public:

protected:
	Sprite* m_pSprite;

	float m_x;
	float m_y;

	float m_velocityX;
	float m_velocityY;

	char m_mask;
	char m_type;

	bool m_dead;

private:

};

#endif //__ENTITY_H__
