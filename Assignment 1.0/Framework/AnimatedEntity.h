#ifndef EXPLOSION_H
#define EXPLOSION_H
#include "entity.h"

class AnimatedEntity : public Entity
{
public:
	AnimatedEntity();
	~AnimatedEntity();
	void Process(float deltaTime);
	void Draw(BackBuffer& backBuffer);
	bool Initialise(AnimatedSprite* animatedSprite);
	void SetExpDead();
	bool IsCollidingWithAnim(AnimatedEntity& e);
	int GetYPos();
	void SetYPos(int yPos);

protected:
private:
public:
protected:
	AnimatedSprite* m_pAnimSprite;
	int yPos;
};
#endif