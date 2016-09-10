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
					game.WolfForm();
				}
				if (e.key.keysym.sym == SDLK_4)
				{
					game.SealForm();
				}
				if (e.key.keysym.sym == SDLK_5)
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
				game.StopVerticalMovement();
			}
			if (e.key.keysym.sym == SDLK_DOWN)
			{
				down = false;
				game.StopVerticalMovement();
			}
			if (e.key.keysym.sym == SDLK_LEFT)
			{
				left = false;
				game.StopHorizontalMovement();
			}
			if (e.key.keysym.sym == SDLK_RIGHT)
			{
				right = false;
				game.StopHorizontalMovement();
			}
		}

		//****************************************************

		if (up == true)
		{
			game.MovePlayerUp('U');
		}
		if (down == true)
		{
			game.MovePlayerDown('D');
		}
		if (left == true)
		{
			game.MovePlayerLeft('L');
		}
		if (right == true)
		{
			game.MovePlayerRight('R');
		}
	}
}
