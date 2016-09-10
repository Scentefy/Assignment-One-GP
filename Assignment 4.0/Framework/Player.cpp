#include "Player.h"

Player::Player()
	:lives(0)
{

}

int 
Player::GetLives()
{
	return lives;
}

void 
Player::SetLives(int l)
{
	lives = l;
}