#include "AnimatedEntity.h"

#include "AnimatedSprite.h"
#include "backbuffer.h"

#include <cassert>


AnimatedEntity::AnimatedEntity(AnimatedSprite* animatedSprite, int x, int y)
{
	this->animatedSprite = animatedSprite;
	animatedSprite->SetX(x);
	animatedSprite->SetY(y);
}


AnimatedEntity::~AnimatedEntity()
{
}

void 
AnimatedEntity::Process(float deltaTime)
{
	animatedSprite->SetX(static_cast<int>(m_x));
	animatedSprite->SetY(static_cast<int>(m_y));
	animatedSprite->Process(deltaTime);
	m_x += m_velocityX;
	m_y += m_velocityY;
		if (m_x > 1000 - 32) 
		{
		m_x = 1000 - 32;
		m_velocityX = 0;
	}
	else if (m_x < 0 + 32) {
		m_x = 0 + 32;
		m_velocityX = 0;
	}

	if (m_y > 600 - 32) {
		m_y = 600 - 32;
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
	animatedSprite->SetX(m_x - (animatedSprite->GetFrameWidth() / 2));
	animatedSprite->SetY(m_y - (animatedSprite->GetFrameHeight() / 2));
	animatedSprite->Draw(backBuffer);
}

bool 
AnimatedEntity::Initialise(AnimatedSprite* animatedSprite)
{
	assert(animatedSprite);
	this->animatedSprite = animatedSprite;

	return (true);
}

void 
AnimatedEntity::SetExpDead()
{
	if (animatedSprite->IsPaused())
	{
		m_dead = true;
	}
}

bool
AnimatedEntity::IsCollidingWithAnim(AnimatedEntity& e)
{
	int r1 = animatedSprite->GetFrameWidth() / 2;
	if (this->IsDead())
		r1 = 0;
	float x1 = GetPositionX() + (animatedSprite->GetFrameWidth() / 2);
	float y1 = GetPositionY() + (animatedSprite->GetFrameHeight() / 2);
	float x2 = e.GetPositionX() + (e.animatedSprite->GetFrameWidth() / 2);
	float y2 = e.GetPositionY() + (e.animatedSprite->GetFrameHeight() / 2);
	int r2 = e.animatedSprite->GetFrameWidth() / 2;
	bool collide = false;
	int r1f = static_cast<int>(r1);
	int r2f = static_cast<int>(r2);
	float collision = r1f + r2f;
	// W02.3: Generic Entity Collision routine.
	float actualdistance = sqrt(pow((x2 - x1), 2) + pow((y2 - y1), 2));
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

int
AnimatedEntity::GetYPos()
{
	return animatedSprite->GetYPos();
}

void
AnimatedEntity::SetYPos(int y)
{
	animatedSprite->SetYPos(y);
}
