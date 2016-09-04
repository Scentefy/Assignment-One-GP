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
#include "Animatedsprite.h"
#include "Player.h"
#include "Enemy.h"

// Static Members:
Game* Game::sm_pInstance = 0;
Player* pPlayer = 0;
Enemy* pEnemy = 0;
TextureManager* texture = 0;
Blood* pExplosive = 0;

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
	const int width = 1000;
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
	pPlayer = new Player();
	pPlayer->Initialise(playerSprite);
	pPlayer->PauseAnimatedSprite();
	playerSprite->SetFrameSpeed(0.4f);
	playerSprite->SetFrameWidth(32);
	playerSprite->SetFrameHeight(48);
	playerSprite->SetNumOfFrames(3);
	playerSprite->SetYPos(0);
	mask = 'N';
	playerSprite->SetLooping(true);
	pPlayer->SetLives(3);
	pPlayer->SetPositionX(400.0f);
	pPlayer->SetPositionY(550.0f);
	pPlayer->SetHorizontalVelocity(0.0f);
	pPlayer->SetVerticalVelocity(0.0f);
	pPlayer->SetDead(false);

	// W02.2: Spawn four rows of 14 alien enemies.
	float x = 400.0f;
	float y = 32.0f;

	for (float i = 1; i <= 5; i++) {
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
	for (size_t i = 0; i < enemyContainer.size(); i++)
	{
		enemyContainer[i]->Process(deltaTime);
	}
	// W02.1: Update player...

	pPlayer->Process(deltaTime);
	// W02.3: For each alien enemy
	// W02.3: Check collision between two entities.
	for (IterationEnemy = enemyContainer.begin(); IterationEnemy < enemyContainer.end();)
		{
			ene = *IterationEnemy;
			if (pPlayer->IsCollidingWithAnim(**IterationEnemy)) {
				float eneX = ene->GetPositionX();
				float eneY = ene->GetPositionY();
				if (mask == 'L')
				{
					ene->SetDead(true);
					delete *IterationEnemy;
					IterationEnemy = enemyContainer.erase(IterationEnemy);
					sound.playSound(soundBlood, false);
				}
				else if (ene->IsCollidingWithAnim(*pPlayer))
				{
					SpawnExplosion(pPlayer->GetPositionX(), pPlayer->GetPositionY());
					sound.playSound(soundBlood, false);
					pPlayer->SetPositionX(400);
					pPlayer->SetPositionY(550);
					pPlayer->SetLives(pPlayer->GetLives()-1);
					if (pPlayer->GetLives() == 0)
						pPlayer->SetDead(true);
				}
				if (ene->IsDead())
					SpawnExplosion(eneX, eneY);

			}
			else
			{
				IterationEnemy++;
			}
	}
	for (size_t i = 0; i < explosionContainer.size(); i++)
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
	// Move Up
	for (size_t i = 0; i < enemyContainer.size(); i++)
	{
		if (enemyContainer[i]->GetPositionY() > 600 - 33)
		{
			enemyContainer[i]->SetVerticalVelocity(-1.0f);
			enemyContainer[i]->SetYPos(144);
		}
		//Move Up
		else if (enemyContainer[i]->GetPositionY() < 33)
		{
			enemyContainer[i]->SetVerticalVelocity(1.0f);
			enemyContainer[i]->SetYPos(0);
		}
	}

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
	
	for (size_t i = 0; i < explosionContainer.size(); i++) {
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
	pPlayer->StartAnimation();
	switch (mask)
	{
	case 'N' :
		pPlayer->SetVerticalVelocity(-2.5f);
		playerSprite->SetYPos(144);
		break;
	case 'B':
		pPlayer->SetVerticalVelocity(-2.5f);
		pBatSprite->SetYPos(144);
		break;
	case 'C':
		pPlayer->SetVerticalVelocity(-2.5f);
		pCatSprite->SetYPos(96);
		break;
	case 'L':
		pPlayer->SetVerticalVelocity(-2.5f);
		pLionSprite->SetYPos(162);
		break;
	case 'Y':
		pPlayer->SetVerticalVelocity(-2.5f);
		pBearSprite->SetYPos(240);
		break;
	case 'W':
		pPlayer->SetVerticalVelocity(-2.5f - 2.5f);
		pWolfSprite->SetYPos(144);
		break;
	case 'S':
		pPlayer->SetVerticalVelocity(-2.5f);
		pSealSprite->SetYPos(144);
		break;
	}
}

void
Game::MovePlayerDown()
{
	pPlayer->StartAnimation();
	switch (mask)
	{
	case 'N' :
		pPlayer->SetVerticalVelocity(2.5f);
		playerSprite->SetYPos(0);
		break;
	case 'B' :
		pPlayer->SetVerticalVelocity(2.5f);
		pBatSprite->SetYPos(0);
		break;
	case 'C':
		pPlayer->SetVerticalVelocity(2.5f);
		pCatSprite->SetYPos(0);
		break;
	case 'L':
		pPlayer->SetVerticalVelocity(2.5f);
		pLionSprite->SetYPos(0);
		break;
	case 'Y':
		pPlayer->SetVerticalVelocity(2.5f);
		pBearSprite->SetYPos(0);
		break;
	case 'W':
		pPlayer->SetVerticalVelocity(2.5f + 2.5f);
		pWolfSprite->SetYPos(0);
		break;
	case 'S':
		pPlayer->SetVerticalVelocity(2.5f);
		pSealSprite->SetYPos(0);
		break;
	}
}

void
Game::MovePlayerLeft()
{
	pPlayer->StartAnimation();
	switch (mask)
	{
	case 'N' :
		pPlayer->SetHorizontalVelocity(-2.5f);
		playerSprite->SetYPos(48);
		break;
	case 'B' :
		pPlayer->SetHorizontalVelocity(-2.5f);
		pBatSprite->SetYPos(48);
		break;
	case 'C':
		pPlayer->SetHorizontalVelocity(-2.5f);
		pCatSprite->SetYPos(32);
		break;
	case 'L':
		pPlayer->SetHorizontalVelocity(-2.5f);
		pLionSprite->SetYPos(53);
		break;
	case 'Y':
		pPlayer->SetHorizontalVelocity(-2.5f);
		pBearSprite->SetYPos(80);
		break;
	case 'W':
		pPlayer->SetHorizontalVelocity(-2.5f - 2.5f);
		pWolfSprite->SetYPos(48);
		break;
	case 'S':
		pPlayer->SetHorizontalVelocity(-2.5f);
		pSealSprite->SetYPos(48);
		break;
	}
}

void
Game::MovePlayerRight()
{
	pPlayer->StartAnimation();
	switch (mask)
	{
	case 'N':
		pPlayer->SetHorizontalVelocity(2.5f);
		playerSprite->SetYPos(96);
		break;
	case 'B':
		pPlayer->SetHorizontalVelocity(2.5f);
		pBatSprite->SetYPos(96);
		break;
	case 'C':
		pPlayer->SetHorizontalVelocity(2.5f);
		pCatSprite->SetYPos(64);
		break;
	case 'L':
		pPlayer->SetHorizontalVelocity(2.5f);
		pLionSprite->SetYPos(106);
		break;
	case 'Y':
		pPlayer->SetHorizontalVelocity(2.5f);
		pBearSprite->SetYPos(160);
		break;
	case 'W':
		pPlayer->SetHorizontalVelocity(2.5f + 2.5f);
		pWolfSprite->SetYPos(96);
		break;
	case 'S':
		pPlayer->SetHorizontalVelocity(2.5f );
		pSealSprite->SetYPos(96);
		break;
	}
}

void 
Game::StopSpaceShipMovementHorizontal()
{
	pPlayer->SetHorizontalVelocity(0.0f);
	PauseAnimation();
}

void
Game::StopSpaceShipMovementVertical()
{
	pPlayer->SetVerticalVelocity(0.0f);
	PauseAnimation();
}

void
Game::PauseAnimation()
{
	if (pPlayer->GetHorizontalVelocity() == 0.0f && pPlayer->GetVerticalVelocity() == 0.0f)
	{
		pPlayer->PauseAnimatedSprite();
	}
}

// W02.2: Spawn a Enemy in game.
void 
Game::SpawnEnemy(float x, float y)
{
	// W02.2: Load the alien enemy sprite file.
	enemySprite = m_pBackBuffer->CreateAnimSprite("assets\\darkknight.png");
	pEnemy = new Enemy();
	pEnemy->Initialise(enemySprite);
	enemySprite->SetFrameSpeed(0.3f);
	enemySprite->SetFrameWidth(32);
	enemySprite->SetFrameHeight(48);
	enemySprite->SetNumOfFrames(3);
	enemySprite->SetYPos(0);
	enemySprite->SetLooping(true);
	pEnemy->SetPositionX(x);
	pEnemy->SetPositionY(y);
	pEnemy->SetHorizontalVelocity(0.0f);
	pEnemy->SetVerticalVelocity(1.0f);
	pEnemy->SetDead(false);

	// W02.2: Add the new Enemy to the enemy container.
	enemyContainer.push_back(pEnemy);
}

void
Game::SpawnExplosion(float x, float y)
{
	AnimatedSprite* pExplosiveSprite = m_pBackBuffer->CreateAnimSprite("assets\\bloodsprite.png");
	pExplosive = new Blood();
	pExplosive->Initialise(pExplosiveSprite);
	pExplosiveSprite->SetFrameSpeed(0.3f);
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
	pPlayer->PauseAnimatedSprite();
	pBatSprite->SetFrameSpeed(0.1f);
	pBatSprite->SetFrameWidth(32);
	pBatSprite->SetFrameHeight(48);
	pBatSprite->SetNumOfFrames(3);
	pBatSprite->SetYPos(0);
	pBatSprite->SetLooping(true);
	pBatSprite->SetX((int)pPlayer->GetPositionX());
	pBatSprite->SetY((int)pPlayer->GetPositionY());
}

void
Game::CatForm()
{
	mask = 'C';
	pCatSprite = m_pBackBuffer->CreateAnimSprite("assets\\cat.png");
	pPlayer->Initialise(pCatSprite);
	pPlayer->PauseAnimatedSprite();
	pCatSprite->SetFrameSpeed(0.2f);
	pCatSprite->SetFrameWidth(32);
	pCatSprite->SetFrameHeight(32);
	pCatSprite->SetNumOfFrames(3);
	pCatSprite->SetYPos(0);
	pCatSprite->SetLooping(true);
	pCatSprite->SetX((int)pPlayer->GetPositionX());
	pCatSprite->SetY((int)pPlayer->GetPositionY());
}

void
Game::LionForm()
{
	mask = 'L';
	pLionSprite = m_pBackBuffer->CreateAnimSprite("assets\\Lion.png");
	pPlayer->Initialise(pLionSprite);
	pPlayer->PauseAnimatedSprite();
	pLionSprite->SetFrameSpeed(0.2f);
	pLionSprite->SetFrameWidth(48);
	pLionSprite->SetFrameHeight(53);
	pLionSprite->SetNumOfFrames(3);
	pLionSprite->SetYPos(0);
	pLionSprite->SetLooping(true);
	pLionSprite->SetX((int)pPlayer->GetPositionX());
	pLionSprite->SetY((int)pPlayer->GetPositionY());
}

void
Game::BearForm()
{
	mask = 'Y';
	pBearSprite = m_pBackBuffer->CreateAnimSprite("assets\\bear.png");
	pPlayer->Initialise(pBearSprite);
	pEnemy->PauseAnimatedSprite();
	pBearSprite->SetFrameSpeed(0.4f);
	pBearSprite->SetFrameWidth(80);
	pBearSprite->SetFrameHeight(80);
	pBearSprite->SetNumOfFrames(3);
	pBearSprite->SetYPos(0);
	pBearSprite->SetLooping(true);
	pBearSprite->SetX((int)pPlayer->GetPositionX());
	pBearSprite->SetY((int)pPlayer->GetPositionY());
}

void
Game::WolfForm()
{
	mask = 'W';
	pWolfSprite = m_pBackBuffer->CreateAnimSprite("assets\\wolf.png");
	pPlayer->Initialise(pWolfSprite);
	pPlayer->PauseAnimatedSprite();
	pWolfSprite->SetFrameSpeed(0.1f);
	pWolfSprite->SetFrameWidth(48);
	pWolfSprite->SetFrameHeight(48);
	pWolfSprite->SetNumOfFrames(3);
	pWolfSprite->SetYPos(0);
	pWolfSprite->SetLooping(true);
	pWolfSprite->SetX((int)pPlayer->GetPositionX());
	pWolfSprite->SetY((int)pPlayer->GetPositionY());
}

void
Game::SealForm()
{
	mask = 'S';
	pSealSprite = m_pBackBuffer->CreateAnimSprite("assets\\seal.png");
	pPlayer->Initialise(pSealSprite);
	pPlayer->PauseAnimatedSprite();
	pSealSprite->SetFrameSpeed(0.3f);
	pSealSprite->SetFrameWidth(48);
	pSealSprite->SetFrameHeight(48);
	pSealSprite->SetNumOfFrames(3);
	pSealSprite->SetYPos(0);
	pSealSprite->SetLooping(true);
	pSealSprite->SetX((int)pPlayer->GetPositionX());
	pSealSprite->SetY((int)pPlayer->GetPositionY());
}

void
Game::HumanForm()
{
	mask = 'N';
	pPlayer->Initialise(playerSprite);
	pPlayer->PauseAnimatedSprite();
	playerSprite->SetFrameSpeed(0.25f);
	playerSprite->SetFrameWidth(32);
	playerSprite->SetFrameHeight(48);
	playerSprite->SetNumOfFrames(3);
	playerSprite->SetYPos(0);
	playerSprite->SetLooping(true);
}

