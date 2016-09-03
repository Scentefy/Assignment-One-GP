#ifndef PLAYER_H
#define PLAYER_H
#include "AnimatedEntity.h"

class Player : public AnimatedEntity
{
public:
	Player();
	~Player();
	
	int GetLives();
	void SetLives(int l);

protected:

private:

public:
	int lives;

protected:

private:
};
#endif