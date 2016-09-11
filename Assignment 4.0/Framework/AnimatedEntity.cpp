#include "AnimatedEntity.h"

#include "AnimatedSprite.h"
#include "backbuffer.h"

#include <cassert>


AnimatedEntity::AnimatedEntity()
: m_pAnimSprite(0)
, yPos(0)
{

}

AnimatedEntity::~AnimatedEntity()
{
}

void 
AnimatedEntity::Process(float deltaTime)
{
	m_pAnimSprite->SetX(static_cast<int>(m_x));
	m_pAnimSprite->SetY(static_cast<int>(m_y));
	m_pAnimSprite->Process(deltaTime);
	m_x += m_velocityX;
	m_y += m_velocityY;
		if (m_x > 1000 - 32) 
		{
		m_x = 1000 - 32;
		m_velocityX = 0;
	}
	else if (m_x < 0 + 28) {
		m_x = 0 + 28;
		m_velocityX = 0;
	}

	if (m_y > 600) {
		m_y = 600;
		m_velocityY = 0;
	}
	else if (m_y < 0 + 32) {
		m_y = 0 + 32;
		m_velocityY = 0;
	}
}

void 
AnimatedEntity::Draw(BackBuffer& backBuffer)
{
	m_pAnimSprite->SetX(m_x - (m_pAnimSprite->GetFrameWidth() / 2));
	m_pAnimSprite->SetY(m_y - (m_pAnimSprite->GetFrameHeight() / 2));
	m_pAnimSprite->Draw(backBuffer);
}

bool 
AnimatedEntity::Initialise(AnimatedSprite* animatedSprite)
{
	assert(animatedSprite);
	this->m_pAnimSprite = animatedSprite;

	return (true);
}

void 
AnimatedEntity::SetExpDead()
{
	if (m_pAnimSprite->IsPaused())
	{
		m_dead = true;
	}
}

bool
AnimatedEntity::IsCollidingWithAnim(AnimatedEntity& e)
{
	int r1 = m_pAnimSprite->GetFrameWidth() / 2;
	if (this->IsDead())
	{
		r1 = 0;
		return false;
	}
	float x1 = GetPositionX();;
	float y1 = GetPositionY();
	float x2 = e.GetPositionX();
	float y2 = e.GetPositionY();
	int r2 = e.m_pAnimSprite->GetFrameWidth() / 2;
	bool collide = false;
	auto collision = r1 + r2;
	// W02.3: Generic Entity Collision routine.
	float actualdistance = sqrt(pow((x2 - x1), 2) + pow((y2 - y1), 2));
	// W02.3: Does this object collide with the e object?
	if (actualdistance < collision)
	{
		collide = true;
	}
	if (collide == true)
	{
		return(collide);
	}

	return (false); // W02.4 Change return value!
}


bool
AnimatedEntity::IsCollidingWithEnt(Entity& e)
{
	int r1 = m_pAnimSprite->GetFrameWidth() / 2;
	if (this->IsDead())
	{
		r1 = 0;
		return false;
	}
	if (this->GetMask() == 'S')
	{
		if (walkableWater == true)
		{
			return false;
		}
	}
	if (this->GetMask() == 'C')
	{
		if (walkableNarrow == true)
		{
			return false;
		}
	}
	if (this->GetMask() == 'B')
	{
		if (walkableLow == true)
		{
			return false;
		}
	}
	auto r12nd = m_pAnimSprite->GetFrameWidth() / 2 + 20;
	float x1 = GetPositionX();
	float y1 = GetPositionY();
	float x2 = e.GetPositionX();
	float y2 = e.GetPositionY();
	auto r2 = e.GetSpriteWidth() / 2 - 5;
	collide = false;
	fakeCollide = r12nd + r2;
	auto collision = r1 + r2;
	auto collisionbig = r12nd + r2;
	// W02.3: Generic Entity Collision routine.
	float actualdistance = sqrt(pow((x2 - x1), 2) + pow((y2 - y1), 2));
	// W02.3: Does this object collide with the e object?
	if (actualdistance < collision)
	{
		collide = true;
		return collide;
	}
	return (false); // W02.4 Change return value!
}

int
AnimatedEntity::GetYPos()
{
	return m_pAnimSprite->GetYPos();
}

void
AnimatedEntity::SetYPos(int y)
{
	m_pAnimSprite->SetYPos(y);
}

bool
AnimatedEntity::GetWalkableWater()
{
	return walkableWater;
}

void
AnimatedEntity::SetWalkableWater(bool w)
{
	walkableWater = w;
}

bool
AnimatedEntity::GetWalkableNarrow()
{
	return walkableNarrow;
}

void
AnimatedEntity::SetWalkableNarrow(bool n)
{
	walkableNarrow = n;
}

bool
AnimatedEntity::GetWalkableLow()
{
	return walkableLow;
}

void
AnimatedEntity::SetWalkableLow(bool l)
{
	walkableLow = l;
}

void
AnimatedEntity::PauseAnimatedSprite()
{
	m_pAnimSprite->Pause(true);
	m_pAnimSprite->SetCurrentFrame(0);
}

void
AnimatedEntity::StartAnimation()
{
	m_pAnimSprite->Pause(false);
}

float
AnimatedEntity::GetCollide()
{
	return actualDistance;
}

int
AnimatedEntity::GetRadius(Entity& e)
{
	return e.GetSpriteWidth();
}