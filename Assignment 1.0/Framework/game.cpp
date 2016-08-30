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
AnimatedEntity* pPlayer = 0;
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
	pPlayer = new AnimatedEntity(playerSprite, 400.0f, 550.0f);
	pPlayer->Initialise(playerSprite);
	playerSprite->SetFrameSpeed(0.4f);
	playerSprite->SetFrameWidth(32);
	playerSprite->SetFrameHeight(48);
	playerSprite->SetNumOfFrames(3);
	playerSprite->SetYPos(0);
	mask = 'N';
	playerSprite->SetLooping(true);
	pPlayer->SetPositionX(400.0f);
	pPlayer->SetPositionY(550.0f);
	pPlayer->SetHorizontalVelocity(0.0f);
	pPlayer->SetVerticalVelocity(0.0f);
	pPlayer->SetDead(false);

	// W02.2: Spawn four rows of 14 alien enemies.
	float x = 400.0f;
	float y = 0.0f;

	//SpawnEnemy(pPlayer->GetPositionX(), pPlayer->GetPositionY()+10);

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

	pPlayer->Process(deltaTime);
	// W02.3: Check for bullet vs alien enemy collisions...
	// W02.3: For each bullet
	// W02.3: For each alien enemy
	// W02.3: Check collision between two entities.

	for (IterationEnemy = enemyContainer.begin(); IterationEnemy < enemyContainer.end();)
		{
			ene = *IterationEnemy;
			if (pPlayer->IsCollidingWithAnim(**IterationEnemy)) {
				int eneX = ene->GetPositionX();
				int eneY = ene->GetPositionY();
				//if (mask == 'L')
				//{
					ene->SetDead(true);
					delete *IterationEnemy;
					IterationEnemy = enemyContainer.erase(IterationEnemy);
					sound.playSound(soundBlood, false);
				//}
				//else if (ene->IsCollidingWithAnim(*pPlayer))
				//{
				//	pPlayer->SetDead(true);
				///}
				if (ene->IsDead())
					SpawnExplosion(eneX, eneY);

			}
			else
			{
				IterationEnemy++;
			}
	}
	for (int i = 0; i < explosionContainer.size(); i++)
	{
		explosionContainer[i]->Process(deltaTime);
	}

	for (IterationDeath = explosionContainer.begin(); IterationDeath < explosionContainer.end();)
	{
		exp = *IterationDeath;
		exp->SetExpDead();
		if (exp->IsDead() == true)
		{
			delete *IterationDeath;
			IterationDeath = explosionContainer.erase(IterationDeath);
		}
		else
			IterationDeath++;
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
	//for (IterationDeath = explosionContainer.begin(); IterationDeath < explosionContainer.end(); i++)
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
	for (IterationEnemy = enemyContainer.begin(); IterationEnemy < enemyContainer.end(); IterationEnemy++, i++)
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
	if (pPlayer->IsDead() == false)
	pPlayer->Draw(backBuffer);
	else
	{
		
	}

	backBuffer.Present();
}

void 
Game::Quit()
{
	m_looping = false;
}

void
Game::MovePlayerUp()
{
	float speed = 0.0f;
	switch (mask)
	{
	case 'N' :
		pPlayer->SetVerticalVelocity(-2.5f - speed);
		playerSprite->SetYPos(144);
		break;
	case 'B':
		pPlayer->SetVerticalVelocity(-2.5f - speed);
		pBatSprite->SetYPos(144);
		break;
	case 'C':
		pPlayer->SetVerticalVelocity(-2.5f - speed);
		pCatSprite->SetYPos(96);
		break;
	case 'L':
		pPlayer->SetVerticalVelocity(-2.5f - speed);
		pLionSprite->SetYPos(162);
		break;
	case 'Y':
		pPlayer->SetVerticalVelocity(-2.5f - speed);
		pBearSprite->SetYPos(240);
		break;
	}
}

void
Game::MovePlayerDown()
{
	float speed = 0.0f;
	switch (mask)
	{
	case 'N' :
		pPlayer->SetVerticalVelocity(2.5f + speed);
		playerSprite->SetYPos(0);
		break;
	case 'B' :
		pPlayer->SetVerticalVelocity(2.5f + speed);
		pBatSprite->SetYPos(0);
		break;
	case 'C':
		pPlayer->SetVerticalVelocity(2.5f + speed);
		pCatSprite->SetYPos(0);
		break;
	case 'L':
		pPlayer->SetVerticalVelocity(2.5f + speed);
		pLionSprite->SetYPos(0);
		break;
	case 'Y':
		pPlayer->SetVerticalVelocity(2.5f + speed);
		pBearSprite->SetYPos(0);
		break;
	}
}

void
Game::MovePlayerLeft()
{
	float speed = 0.0f;
	switch (mask)
	{
	case 'N' :
		pPlayer->SetHorizontalVelocity(-2.5f - speed);
		playerSprite->SetYPos(48);
		break;
	case 'B' :
		pPlayer->SetHorizontalVelocity(-2.5f - speed);
		pBatSprite->SetYPos(48);
		break;
	case 'C':
		pPlayer->SetHorizontalVelocity(-2.5f - speed);
		pCatSprite->SetYPos(32);
		break;
	case 'L':
		pPlayer->SetHorizontalVelocity(-2.5f - speed);
		pLionSprite->SetYPos(53);
		break;
	case 'Y':
		pPlayer->SetHorizontalVelocity(-2.5f - speed);
		pBearSprite->SetYPos(80);
		break;
	}
}

void
Game::MovePlayerRight()
{
	float speed = 0.0f;
	switch (mask)
	{
	case 'N':
		pPlayer->SetHorizontalVelocity(2.5f + speed);
		playerSprite->SetYPos(96);
		break;
	case 'B':
		pPlayer->SetHorizontalVelocity(2.5f + speed);
		pBatSprite->SetYPos(96);
		break;
	case 'C':
		pPlayer->SetHorizontalVelocity(2.5f + speed);
		pCatSprite->SetYPos(64);
		break;
	case 'L':
		pPlayer->SetHorizontalVelocity(2.5f + speed);
		pLionSprite->SetYPos(106);
		break;
	case 'Y':
		pPlayer->SetHorizontalVelocity(2.5f + speed);
		pBearSprite->SetYPos(160);
		break;
	}
}

void 
Game::StopSpaceShipMovementHorizontal()
{
	pPlayer->SetHorizontalVelocity(0.0f);
	//playerSprite->Pause();
}

void
Game::StopSpaceShipMovementVertical()
{
	pPlayer->SetVerticalVelocity(0.0f);
	//playerSprite->Pause();
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
	AnimatedSprite* pExplosiveSprite = m_pBackBuffer->CreateAnimSprite("assets\\bloodsprite.png");
	pExplosive = new AnimatedEntity(pExplosiveSprite,x ,y);
	pExplosive->Initialise(pExplosiveSprite);
	pExplosiveSprite->SetFrameSpeed(0.4f);
	pExplosiveSprite->SetFrameWidth(64);
	pExplosiveSprite->SetFrameHeight(64);
	pExplosiveSprite->SetNumOfFrames(5);
	pExplosiveSprite->SetYPos(0);
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
	pPlayer->Initialise(pBatSprite);
	pBatSprite->SetFrameSpeed(0.4f);
	pBatSprite->SetFrameWidth(32);
	pBatSprite->SetFrameHeight(48);
	pBatSprite->SetNumOfFrames(3);
	pBatSprite->SetYPos(0);
	pBatSprite->SetLooping(true);
	pBatSprite->SetX(pPlayer->GetPositionX());
	pBatSprite->SetY(pPlayer->GetPositionY());
}

void
Game::CatForm()
{
	mask = 'C';
	pCatSprite = m_pBackBuffer->CreateAnimSprite("assets\\cat.png");
	pPlayer->Initialise(pCatSprite);
	pCatSprite->SetFrameSpeed(0.4f);
	pCatSprite->SetFrameWidth(32);
	pCatSprite->SetFrameHeight(32);
	pCatSprite->SetNumOfFrames(3);
	pCatSprite->SetYPos(0);
	pCatSprite->SetLooping(true);
	pCatSprite->SetX(pPlayer->GetPositionX());
	pCatSprite->SetY(pPlayer->GetPositionY());
}

void
Game::LionForm()
{
	mask = 'L';
	pLionSprite = m_pBackBuffer->CreateAnimSprite("assets\\Lion.png");
	pPlayer->Initialise(pLionSprite);
	pLionSprite->SetFrameSpeed(0.4f);
	pLionSprite->SetFrameWidth(48);
	pLionSprite->SetFrameHeight(53);
	pLionSprite->SetNumOfFrames(3);
	pLionSprite->SetYPos(0);
	pLionSprite->SetLooping(true);
	pLionSprite->SetX(pPlayer->GetPositionX());
	pLionSprite->SetY(pPlayer->GetPositionY());
}

void
Game::BearForm()
{
	mask = 'Y';
	pBearSprite = m_pBackBuffer->CreateAnimSprite("assets\\bear.png");
	pPlayer->Initialise(pBearSprite);
	pBearSprite->SetFrameSpeed(0.4f);
	pBearSprite->SetFrameWidth(80);
	pBearSprite->SetFrameHeight(80);
	pBearSprite->SetNumOfFrames(3);
	pBearSprite->SetYPos(0);
	pBearSprite->SetLooping(true);
	pBearSprite->SetX(pPlayer->GetPositionX());
	pBearSprite->SetY(pPlayer->GetPositionY());
}

void
Game::HumanForm()
{
	mask = 'N';
	pPlayer->Initialise(playerSprite);
	playerSprite->SetFrameSpeed(0.4f);
	playerSprite->SetFrameWidth(32);
	playerSprite->SetFrameHeight(48);
	playerSprite->SetNumOfFrames(3);
	playerSprite->SetYPos(0);
	playerSprite->SetLooping(true);
}

