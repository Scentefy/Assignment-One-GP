#ifndef EXPLOSION_H
#define EXPLOSION_H
#include "entity.h"

class AnimatedEntity : public Entity
{
public:
	AnimatedEntity(AnimatedSprite* animatedSprite, int x, int y);
	void Process(float deltaTime);
	void Draw(BackBuffer& backBuffer);
	bool Initialise(AnimatedSprite* animatedSprite);
	void SetExpDead();
	bool IsCollidingWithAnim(AnimatedEntity& e);
	int GetYPos();
	void SetYPos(int y);

protected:
private:
public:
protected:
	AnimatedSprite* animatedSprite;
};
#endif