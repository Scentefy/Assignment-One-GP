// COMP710 GP 2D Framework

// This includes:
#include "game.h"

// Local includes:
#include "backbuffer.h"
#include "inputhandler.h"
#include "logmanager.h"
#include "sprite.h"
#include "SoundSys.h"

// Library includes:
#include <cassert>
#include <SDL.h>
#include <vector>
#include "Bullet.h"
#include "Animatedsprite.h"

// Static Members:
Game* Game::sm_pInstance = 0;
AnimatedEntity* pPlayerShip = 0;
AnimatedEntity* pEnemy = 0;
Bullet* pBullet = 0;
TextureManager* texture = 0;
AnimatedEntity* pExplosive = 0;

Game&
Game::GetInstance()
{
	if (sm_pInstance == 0)
	{ 
		sm_pInstance = new Game();
	}

	assert(sm_pInstance);

	return (*sm_pInstance);
}

void 
Game::DestroyInstance()
{
	delete sm_pInstance; 
	sm_pInstance = 0;
}

Game::Game()
: m_pBackBuffer(0)
, m_pInputHandler(0)
, m_looping(true)
, m_executionTime(0)
, m_elapsedSeconds(0)
, m_frameCount(0)
, m_FPS(0)
, m_numUpdates(0)
, m_lastTime(0)
, m_lag(0)
{
	
}

Game::~Game()
{
	delete m_pBackBuffer;
	m_pBackBuffer = 0;
}

bool 
Game::Initialise()
{
	const int width = 800;
	const int height = 600;

	m_pBackBuffer = new BackBuffer();
	if (!m_pBackBuffer->Initialise(width, height))
	{
		LogManager::GetInstance().Log("BackBuffer Init Fail!");
		return (false);
	}

	m_pInputHandler = new InputHandler();
	if (!m_pInputHandler->Initialise())
	{
		LogManager::GetInstance().Log("InputHandler Init Fail!");
		return (false);
	}
	else {
		LogManager::GetInstance().Log("InputHandler Init Success!");

	}

	backGround = m_pBackBuffer->CreateSprite("assets\\RPGScreenshot.png");
	backGround->SetX(200);
	backGround->SetY(150);


	sound.createSound(&soundBlood, "assets\\blood.mp3");


	// W02.1: Load the player ship sprite.
	playerSprite = m_pBackBuffer->CreateAnimSprite("assets\\steampunk.png");
	// W02.1: Create the player ship instance.
	pPlayerShip = new AnimatedEntity(playerSprite, 400.0f, 550.0f);
	pPlayerShip->Initialise(playerSprite);
	playerSprite->SetFrameSpeed(0.4f);
	playerSprite->SetFrameWidth(32);
	playerSprite->SetFrameHeight(48);
	playerSprite->SetNumOfFrames(3);
	playerSprite->SetYPos(0);
	mask = 'N';
	playerSprite->SetLooping(true);
	pPlayerShip->SetPositionX(400.0f);
	pPlayerShip->SetPositionY(550.0f);
	pPlayerShip->SetHorizontalVelocity(0.0f);
	pPlayerShip->SetVerticalVelocity(0.0f);
	pPlayerShip->SetDead(false);
	Mask cMask;

	// W02.2: Spawn four rows of 14 alien enemies.
	float x = 400.0f;
	float y = 0.0f;

	//SpawnEnemy(pPlayerShip->GetPositionX(), pPlayerShip->GetPositionY()+10);

	for (float i = 1; i <= 4; i++) {
		for (float j = 1; j <= 5; j++) {
			SpawnEnemy(x, y);
			x += 55;
		}
		y += 50.0f;
		x = 20.0f;
	}

	m_lastTime = SDL_GetTicks();
	m_lag = 0.0f;

	return (true);
}

bool 
Game::DoGameLoop()
{
	const float stepSize = 1.0f / 60.0f;

	assert(m_pInputHandler);
	m_pInputHandler->ProcessInput(*this);
	
	if (m_looping)
	{
		int current = SDL_GetTicks();
		float deltaTime = (current - m_lastTime) / 1000.0f;
		m_lastTime = current;

		m_executionTime += deltaTime;

		m_lag += deltaTime;

		while (m_lag >= stepSize)
		{
			Process(stepSize);

			m_lag -= stepSize;

			++m_numUpdates;
		}
		
		Draw(*m_pBackBuffer);
	}

	SDL_Delay(1);

	return (m_looping);
}

void 
Game::Process(float deltaTime)
{
	// Count total simulation time elapsed:
	m_elapsedSeconds += deltaTime;

	// Frame Counter:
	if (m_elapsedSeconds > 1)
	{
		m_elapsedSeconds -= 1;
		m_FPS = m_frameCount;
		m_frameCount = 0;
	}

	// Update the game world simulation:

	// Ex003.5: Process each alien enemy in the container.
	for (int i = 0; i < enemyContainer.size(); i++)
	{
		enemyContainer[i]->Process(deltaTime);
	}
	// W02.3: Process each bullet in the container.
	for (int i = 0; i < bulletContainer.size(); i++)
	{
		bulletContainer[i]->Process(deltaTime);
	}
	// W02.1: Update player...

	pPlayerShip->Process(deltaTime);
	// W02.3: Check for bullet vs alien enemy collisions...
	// W02.3: For each bullet
	// W02.3: For each alien enemy
	// W02.3: Check collision between two entities.

	bool hitEnemy = false;
	//for (iterb = bulletContainer.begin(); iterb < bulletContainer.end();)
	//{
		for (iter = enemyContainer.begin(); iter < enemyContainer.end();)
		{
			ene = *iter;
			if (pPlayerShip->IsCollidingWithAnim(**iter)) {
				LogManager::GetInstance().Log("HiT!");
				int eneX = ene->GetPositionX()-15;
				int eneY = ene->GetPositionY()-15;
				delete *iter;
				iter = enemyContainer.erase(iter);
				SpawnExplosion(eneX, eneY);
				hitEnemy = true;
				sound.playSound(soundBlood, false);
			}
			else
			{
				iter++;
			}
		//}
		//if (hitEnemy || bull->GetPositionY() < 0) {
		//	delete *iterb;
		//	iterb = bulletContainer.erase(iterb);
		//	hitEnemy = false;
		//}
		//else
		//	iterb++;
	}
	for (int i = 0; i < explosionContainer.size(); i++)
	{
		explosionContainer[i]->Process(deltaTime);
	}

	for (itere = explosionContainer.begin(); itere < explosionContainer.end();)
	{
		exp = *itere;
		exp->SetExpDead();
		if (exp->IsDead() == true)
		{
			delete *itere;
			itere = explosionContainer.erase(itere);
		}
		else
			itere++;
	}

	for (int i = 0; i < enemyContainer.size(); i++)
	{
		if (enemyContainer[i]->GetPositionY() > 500)
		{
			enemyContainer[i]->SetVerticalVelocity(-1.0f);
			enemyContainer[i]->SetYPos(144);
		}
		else if (enemyContainer[i]->GetPositionY() < 30)
		{
			enemyContainer[i]->SetVerticalVelocity(1.0f);
			enemyContainer[i]->SetYPos(0);
		}
	}
	//}
	// W02.3: If collided, destory both and spawn explosion.
	//for (itere = explosionContainer.begin(); itere < explosionContainer.end(); i++)
	//{
	//	if (explosionContainer[i]->IsDead())
	//	{
	//		
	//	}
	//}
	// W02.3: Remove any dead bullets from the container...

	// W02.3: Remove any dead enemy aliens from the container...

	// W02.3: Remove any dead explosions from the container...
}

void 
Game::Draw(BackBuffer& backBuffer)
{
	++m_frameCount;
	
	//backBuffer.SetClearColour(255, 0, 0);
	backBuffer.Clear();
	
	backGround->Draw(backBuffer);
	
	// W02.2: Draw all enemy aliens in container...
	int i = 0;
	for (iter = enemyContainer.begin(); iter < enemyContainer.end(); iter++, i++)
	{
		enemyContainer[i]->Draw(backBuffer);
	}
	// W02.3: Draw all bullets in container...
	for (int i = 0; i < bulletContainer.size(); i++) {
		bulletContainer[i]->Draw(backBuffer);
	}
	
	for (int i = 0; i < explosionContainer.size(); i++) {
		explosionContainer[i]->Draw(backBuffer);
	}

	// W02.1: Draw the player ship...
	pPlayerShip->Draw(backBuffer);

	backBuffer.Present();
}

void 
Game::Quit()
{
	m_looping = false;
}

void
Game::MovePlayer(std::string direction)
{
	float speed = 0.0f;

	switch (mask)
	{
	case 'N' :
	if (direction == "up")
	{
		pPlayerShip->SetVerticalVelocity(-2.5f - speed);
			playerSprite->SetYPos(144);
		//else if (bat == true)
		//	batSprite->SetYPos(144);
	}
	else if (direction == "down")
	{
		pPlayerShip->SetVerticalVelocity(2.5f + speed);
			playerSprite->SetYPos(0);
		//else if (bat == true)
		//	batSprite->SetYPos(0);
	}
	else if (direction == "left")
	{
		pPlayerShip->SetHorizontalVelocity(-2.5f - speed);
			playerSprite->SetYPos(48);
		//else if (bat == true)
		//	batSprite->SetYPos(48);
	}
	else if (direction == "right")
	{
		pPlayerShip->SetHorizontalVelocity(2.5f + speed);
			playerSprite->SetYPos(96);
		//else if (bat == true)
		//	batSprite->SetYPos(96);
	}
	break;
	case 'B':
		if (direction == "up")
		{
			pPlayerShip->SetVerticalVelocity(-2.5f - speed);
				pBatSprite->SetYPos(144);
		}
		else if (direction == "down")
		{
			pPlayerShip->SetVerticalVelocity(2.5f + speed);
				pBatSprite->SetYPos(0);
		}
		else if (direction == "left")
		{
			pPlayerShip->SetHorizontalVelocity(-2.5f - speed);
				pBatSprite->SetYPos(48);
		}
		else if (direction == "right")
		{
			pPlayerShip->SetHorizontalVelocity(2.5f + speed);
				pBatSprite->SetYPos(96);
		}
	}
}

void 
Game::StopSpaceShipMovement()
{
	pPlayerShip->SetHorizontalVelocity(0.0f);
	pPlayerShip->SetVerticalVelocity(0.0f);
	//playerSprite->Pause();
}

// W02.3: Space a Bullet in game.
void 
Game::FireSpaceShipBullet()
{
	// W02.3: Load the player bullet sprite.  
	Sprite* bulletSprite = m_pBackBuffer->CreateSprite("assets\\playerbullet.png");

	// W02.3: Create a new bullet object.
	pBullet = new Bullet();
	pBullet->Initialise(bulletSprite);
	pBullet->SetPositionX(pPlayerShip->GetPositionX()+8.0f);
	pBullet->SetPositionY(pPlayerShip->GetPositionY());
	pBullet->SetDead(false);

	// W02.3: Set the bullets vertical velocity.
	pBullet->SetVerticalVelocity(-10.0f);

	// W02.3: Add the new bullet to the bullet container.
	bulletContainer.push_back(pBullet);
}

// W02.2: Spawn a Enemy in game.
void 
Game::SpawnEnemy(float x, float y)
{
	int xi = static_cast<int>(x);
	int yi = static_cast<int>(y);
	// W02.2: Load the alien enemy sprite file.
	enemySprite = m_pBackBuffer->CreateAnimSprite("assets\\darkknight.png");
	pEnemy = new AnimatedEntity(enemySprite,x,y);
	pEnemy->Initialise(enemySprite);
	enemySprite->SetFrameSpeed(0.4f);
	enemySprite->SetFrameWidth(32);
	enemySprite->SetFrameHeight(48);
	enemySprite->SetNumOfFrames(3);
	enemySprite->SetYPos(0);
	enemySprite->SetLooping(true);
	pEnemy->SetPositionX(x);
	pEnemy->SetPositionY(y);
	enemySprite->SetX(xi);
	enemySprite->SetY(yi);
	pEnemy->SetHorizontalVelocity(0.0f);
	pEnemy->SetVerticalVelocity(1.0f);
	pEnemy->SetDead(false);

	// W02.2: Add the new Enemy to the enemy container.
	enemyContainer.push_back(pEnemy);
}

void
Game::SpawnExplosion(int x, int y)
{
	AnimatedSprite* pExplosiveSprite = m_pBackBuffer->CreateAnimSprite("assets\\explosion.png");
	pExplosive = new AnimatedEntity(pExplosiveSprite,x ,y);
	pExplosive->Initialise(pExplosiveSprite);
	pExplosiveSprite->SetFrameSpeed(0.4f);
	pExplosiveSprite->SetFrameWidth(64);
	pExplosiveSprite->SetFrameHeight(64);
	pExplosiveSprite->SetNumOfFrames(4);
	pExplosiveSprite->SetYPos(0);
	//pExplosiveSprite->SetX(static_cast<int>(x));
	//pExplosiveSprite->SetY(static_cast<int>(y));
	pExplosive->SetPositionX(x);
	pExplosive->SetPositionY(y);
	pExplosive->SetDead(false);
	explosionContainer.push_back(pExplosive);
}

void 
Game::BatForm()
{
	mask = 'B';
	pBatSprite = m_pBackBuffer->CreateAnimSprite("assets\\pinkbat.png");
	pPlayerShip->Initialise(pBatSprite);
	pBatSprite->SetFrameSpeed(0.4f);
	pBatSprite->SetFrameWidth(32);
	pBatSprite->SetFrameHeight(48);
	pBatSprite->SetNumOfFrames(3);
	pBatSprite->SetYPos(0);
	pBatSprite->SetLooping(true);
	pBatSprite->SetX(pPlayerShip->GetPositionX());
	pBatSprite->SetY(pPlayerShip->GetPositionY());
}

void
Game::HumanForm()
{
	mask = 'N';
	pPlayerShip->Initialise(playerSprite);
	playerSprite->SetFrameSpeed(0.4f);
	playerSprite->SetFrameWidth(32);
	playerSprite->SetFrameHeight(48);
	playerSprite->SetNumOfFrames(3);
	playerSprite->SetYPos(0);
	playerSprite->SetLooping(true);

}

