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
	bool IsCollidingWithEnt(Entity& e);

	int GetYPos();
	void SetYPos(int yPos);

	bool GetWalkableWater();
	void SetWalkableWater(bool w);

	bool GetWalkableNarrow();
	void SetWalkableNarrow(bool n);

	bool GetWalkableLow();
	void SetWalkableLow(bool l);

	bool GetWalkableWall();
	void SetWalkableWall(bool z);

	void PauseAnimatedSprite();
	void StartAnimation();

	float GetCollide();
	int GetRadius(Entity& e);
protected:
private:
public:
protected:
	AnimatedSprite* m_pAnimSprite;
	int yPos;
	bool walkableWater, walkableNarrow, walkableLow, walkableWall;
	bool collide;
	int fakeCollide;
	float actualDistance;
};
#endif