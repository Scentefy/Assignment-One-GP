// COMP710 GP 2D Framework

// This include:
#include "entity.h"

// Local includes:
#include "sprite.h"
#include "AnimatedSprite.h"
#include "backbuffer.h"

// Library includes:
#include <cassert>
#include <algorithm>

Entity::Entity()
: m_pSprite(0)
, m_pAnimSprite(0)
, m_x(0.0f)
, m_y(0.0f)
, m_velocityX(0.0f)
, m_velocityY(0.0f)
, m_dead(false)
{

}

Entity::~Entity()
{
}

bool
Entity::Initialise(Sprite* sprite)
{
	assert(sprite);
	m_pSprite = sprite;

	return (true);
}

void 
Entity::Process(float deltaTime)
{
	m_pSprite->SetX(static_cast<int>(m_x));
	m_pSprite->SetY(static_cast<int>(m_y));
	// W02.1: Generic position update, based upon velocity (and time).
	m_x += m_velocityX;
	m_y += m_velocityY;
	// W02.1: Boundary checking and position capping. 
	if (m_x > 800 - 32) {
		m_x = 800 - 32;
		m_velocityX = 0;
	}
	else if (m_x < 0) {
		m_x = 0;
		m_velocityX = 0;
	}
}

void 
Entity::Draw(BackBuffer& backBuffer)
{
	assert(m_pSprite);
	m_pSprite->SetX(m_x);
	m_pSprite->SetY(m_y);
	m_pSprite->Draw(backBuffer);
}

bool
Entity::IsCollidingWith(Entity& e)
{

	int r1 = m_pSprite->GetHeight()/2;
	float x1 = GetPositionX();
	float y1 = GetPositionY();
	float x2 = e.GetPositionX();
	float y2 = e.GetPositionY();
	int r2 = e.m_pSprite->GetHeight()/2;
	bool collide = false;
	int r1f = static_cast<int>(r1);
	int r2f = static_cast<int>(r2);
	float collision = r1f + r2f;
	// W02.3: Generic Entity Collision routine.
	float actualdistance = sqrt(pow((x2 - x1),2) + pow((y2 - y1),2));
	// W02.3: Does this object collide with the e object?
	if (actualdistance < collision)
	{
		collide = true;
	}
	// W02.3: Create a circle for each entity (this and e).
	
	// W02.3: Check for intersection.
	// W02.3: Using circle-vs-circle collision detection.

	// W02.3: Return result of collision.
	if (collide == true)
	{
		return(collide);
	}

	return (false); // W02.4 Change return value!
}

void 
Entity::SetDead(bool dead)
{
	m_dead = dead;
}

bool Entity::IsDead() const
{
	return m_dead;
}


float 
Entity::GetPositionX()
{
	return (m_x);
}

float 
Entity::GetPositionY()
{
	return (m_y);
}

void
Entity::SetPositionX(float x)
{
	m_x = x;
}

void
Entity::SetPositionY(float y)
{
	m_y = y;
}

float 
Entity::GetHorizontalVelocity()
{
	return (m_velocityX);
}

float 
Entity::GetVerticalVelocity()
{
	return (m_velocityY);
}

void 
Entity::SetHorizontalVelocity(float x)
{
	m_velocityX = x;
}

void 
Entity::SetVerticalVelocity(float y)
{
	m_velocityY = y;
}
