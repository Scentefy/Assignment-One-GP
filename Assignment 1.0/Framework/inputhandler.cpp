// COMP710 GP 2D Framework

// This include:
#include "inputhandler.h"

// Local includes:
#include "game.h"

// Library includes:
#include <cassert>

InputHandler::InputHandler()
	: m_pGameController(0)
{

}

InputHandler::~InputHandler()
{
	if (m_pGameController)
	{
		SDL_JoystickClose(m_pGameController);
		m_pGameController = 0;
	}
}

bool
InputHandler::Initialise()
{
	int numControllesr = SDL_NumJoysticks();

	m_pGameController = SDL_JoystickOpen(0);
	up = false;
	down = false;
	left = false;
	right = false;

	return (true);
}

void
InputHandler::ProcessInput(Game& game)
{
	// W02.1: Receive Input Events below...
	SDL_Event e;
	while (SDL_PollEvent(&e) != 0)
	{

		if (e.type == SDL_QUIT)
		{
			game.Quit();
		}
		else if (e.type == SDL_KEYDOWN)
		{

			//****************************************************
			if (e.key.keysym.sym == SDLK_UP)
			{
				up = true;
			}
			if (e.key.keysym.sym == SDLK_DOWN)
			{
				down = true;
			}
			if (e.key.keysym.sym == SDLK_LEFT)
			{
				left = true;
			}
			if (e.key.keysym.sym == SDLK_RIGHT)
			{
				right = true;
			}
			if (e.key.keysym.sym == SDLK_1) 
			{
				game.BatForm();
			}
			if (e.key.keysym.sym == SDLK_2)
			{
				game.CatForm();
			}
			if (e.key.keysym.sym == SDLK_3)
			{
				game.LionForm();
			}
			if (e.key.keysym.sym == SDLK_4)
			{
				game.BearForm();
			}
			if (e.key.keysym.sym == SDLK_5)
			{
				game.WolfForm();
			}
			if (e.key.keysym.sym == SDLK_7)
			{
				game.HumanForm();
			}
		}

		//****************************************************
		else if (e.type == SDL_KEYUP)
		{
			if (e.key.keysym.sym == SDLK_UP)
			{
				up = false;
				game.StopSpaceShipMovementVertical();
			}
			if (e.key.keysym.sym == SDLK_DOWN)
			{
				down = false;
				game.StopSpaceShipMovementVertical();
			}
			if (e.key.keysym.sym == SDLK_LEFT)
			{
				left = false;
				game.StopSpaceShipMovementHorizontal();
			}
			if (e.key.keysym.sym == SDLK_RIGHT)
			{
				right = false;
				game.StopSpaceShipMovementHorizontal();
			}
		}

		//****************************************************

		if (up == true)
		{
			game.MovePlayerUp();
		}
		if (down == true)
		{
			game.MovePlayerDown();
		}
		if (left == true)
		{
			game.MovePlayerLeft();
		}
		if (right == true)
		{
			game.MovePlayerRight();
		}
		//else if (e.type == SDL_KEYDOWN)
		//{
		//	// W02.3: Tell the game to fire a player bullet...

		//	if (e.key.keysym.sym == SDLK_LEFT) {
		//		game.MovePlayer("left");
		//	}
		//	if (e.key.keysym.sym == SDLK_RIGHT) {
		//		game.MovePlayer("right");
		//	}
		//	if (e.key.keysym.sym == SDLK_UP) {
		//		game.MovePlayer("up");
		//	}
		//	if (e.key.keysym.sym == SDLK_DOWN) {
		//		game.MovePlayer("down");
		//	}
		//}
		//else if (e.type == SDL_KEYUP)
		//{
		//	if (e.key.keysym.sym == SDLK_LEFT || e.key.keysym.sym == SDLK_RIGHT || e.key.keysym.sym == SDLK_UP || e.key.keysym.sym == SDLK_DOWN)
		//	{
		//		game.StopSpaceShipMovement();
		//	}
		//}
	}
	//	 W02.1: Receive Input Events below...
	//	SDL_Event e;
	//	while (SDL_PollEvent(&e) != 0)
	//	{
	//		if (e.type == SDL_QUIT)
	//		{
	//			game.Quit();
	//		}
	//		else if (e.type == SDL_JOYBUTTONDOWN)
	//		{
	//			 W02.3: Tell the game to fire a player bullet...
	//			if (e.jbutton.button == 10)
	//			{
	//				game.FireSpaceShipBullet();
	//			}
	//
	//			 W02.1: Tell the game to move the space ship left...
	//			if (e.jbutton.button == 2)
	//			{
	//				game.MovePlayerLeft();
	//			}
	//
	//			 W02.1: Tell the game to move the space ship right...
	//			if (e.jbutton.button == 3)
	//			{
	//				game.MovePlayerRight();
	//			}
	//		}
	//		else if (e.type == SDL_JOYBUTTONUP)
	//		{
	//			if (e.jbutton.button == 2 || e.jbutton.button == 3)
	//			{
	//				game.StopPlayerMovement();
	//			}
	//		}
	//	}
}
