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
#include <string>
#include "Animatedsprite.h"
#include "Player.h"
#include "Enemy.h"
#include "Narrow.h"
#include "Parser.h"

// Static Members:
Game* Game::sm_pInstance = 0;
Player* pPlayer = 0;
Enemy* pEnemy = 0;
TextureManager* texture = 0;
Blood* pExplosive = 0;
Tiles* pTile = 0;
WaterTile* pWaterTile = 0;
Tiles* pWall = 0;
LavaTile* pLava = 0;
Narrow* pNarrow = 0;
LowWall* pLow = 0;

//Sprites
AnimatedSprite* playerSprite;
AnimatedSprite* enemySprite;
AnimatedSprite* pBatSprite;
AnimatedSprite* pCatSprite;
AnimatedSprite* pWolfSprite;
AnimatedSprite* pSealSprite;

Sprite* pTileSprite;
Sprite* pTileWaterSprite;
Sprite* pWallSprite;
Sprite* pLavaSprite;
Sprite* pNarrowSprite;
Sprite* pLowWallSprite;

//Deletion Iterators
std::vector<Enemy*>::iterator IterationEnemy;
std::vector <Blood*>::iterator IterationDeath;
std::vector <Tiles*>::iterator IterationTile;
std::vector <WaterTile*>::iterator IterationWaterTile;
std::vector <Tiles*>::iterator IterationWall;
std::vector <LavaTile*>::iterator IterationLavaTile;
std::vector <Narrow*>::iterator IterationNarrow;
std::vector <LowWall*>::iterator IterationLowWall;

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
	Parser::GetInstance().loadInFile("data.ini");
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

	sound.createSound(&soundBlood, "assets\\blood.mp3");

	const Value& playerData = Parser::GetInstance().document["player"];
	// W02.1: Load the player ship sprite.
	playerSprite = m_pBackBuffer->CreateAnimSprite(playerData["sprite_location"].GetString());
	pPlayer = new Player();
	pPlayer->Initialise(playerSprite);
	pPlayer->PauseAnimatedSprite();
	playerSprite->SetFrameSpeed(playerData["frame_speed"].GetFloat());
	playerSprite->SetFrameWidth(playerData["frame_width"].GetInt());
	playerSprite->SetFrameHeight(playerData["frame_height"].GetInt());
	playerSprite->SetNumOfFrames(playerData["num_frames"].GetInt());
	playerSprite->SetYPos(playerData["YPosition"].GetInt());
	pPlayer->SetMask(toChar(playerData["Mask"].GetString()));
	mask = toChar(playerData["Mask"].GetString());
	playerSprite->SetLooping(playerData["looping"].GetBool());
	pPlayer->SetLives(playerData["Lives"].GetInt());
	pPlayer->SetPositionX(playerData["PositionX"].GetFloat());
	pPlayer->SetPositionY(playerData["PositionY"].GetFloat());
	pPlayer->SetHorizontalVelocity(playerData["HorizontalVelo"].GetFloat());
	pPlayer->SetVerticalVelocity(playerData["VerticalVelo"].GetFloat());
	pPlayer->SetDead(playerData["Dead"].GetBool());

	const Value& mapData = Parser::GetInstance().document["map"];
	string map = mapData["mapcreation"].GetString();
	std::vector<char> data(map.begin(), map.end()); 

	float x = 32.0f;
	float y = 41.0f;
	int tile = 0;

	for (int i = 0; i < data.size(); i++)
	{
		//Normal Tile
		if (data[i] == 'N')
		{
			CreateTile(x,y);
			tile++;
		}
		else if (data[i] == 'W')
		{
			CreateWall(x, y);
			tile++;
		}
		else if (data[i] == 'L')
		{
			CreateNarrow(x, y);
			tile++;
		}
		else if (data[i] == 'E')
		{
			CreateLavaTile(x, y);
			tile++;
		}
		else if (data[i] == 'K')
		{
			CreateLowWall(x, y);
			tile++;
		}
		else if (data[i] == 'U')
		{
			CreateWaterTile(x, y);
			tile++;
		}
		x += 32.0f;
		if (tile == 30)
		{
			tile = 0;
			y += 32.0f;
			x = 32.0f;
		}
	}

	// W02.2: Spawn four rows of 14 alien enemies.
	//float enemyX = 400.0f;
	//float enemyY = 32.0f;

	//for (float i = 1; i <= 5; i++) {
	//	for (float j = 1; j <= 5; j++) {
	//		SpawnEnemy(enemyX, enemyY);
	//		enemyX += 55;
	//	}
	//	enemyY += 50.0f;
	//	enemyX = 20.0f;
	//}

	//auto tileX = 0.0f;
	//auto tileY = 9.0f;
	//for (float i = 1; i <= 19; i++) {
	//	for (float j = 1; j <= 32; j++) {
	//		CreateTile(tileX, tileY);
	//		tileX += 32;
	//	}
	//	tileY += 32.0f;
	//	tileX = 0.0f;
	//}

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

	float oldPositionX = 0;
	float oldPositionY = 0;
	if (deltaTime - oldDelta > 50)
		oldDelta = deltaTime;
	if (oldPositionX != pPlayer->GetPositionX() && oldPositionY != pPlayer->GetPositionY())
	{
		oldPositionX = pPlayer->GetPositionX();
		oldPositionY = pPlayer->GetPositionY();
	}

	for (size_t i = 0; i < enemyContainer.size(); i++)
	{
		enemyContainer[i]->Process(deltaTime);
	}

	for (size_t i = 0; i < tileContainer.size(); i++)
	{
		tileContainer[i]->Process(deltaTime);
	}

	for (size_t i = 0; i < waterTileContainer.size(); i++)
	{
		waterTileContainer[i]->Process(deltaTime);
	}

	for (size_t i = 0; i < wallContainer.size(); i++)
	{
		wallContainer[i]->Process(deltaTime);
	}

	for (size_t i = 0; i < NarrowTileContainer.size(); i++)
	{
		NarrowTileContainer[i]->Process(deltaTime);
	}

	for (size_t i = 0; i < LowWallTileContainer.size(); i++)
	{
		LowWallTileContainer[i]->Process(deltaTime);
	}

	for (size_t i = 0; i < lavaTileContainer.size(); i++)
	{
		lavaTileContainer[i]->Process(deltaTime);
	}

	//Player Process
	pPlayer->Process(deltaTime);

	//Collision with Enemy
	for (IterationEnemy = enemyContainer.begin(); IterationEnemy < enemyContainer.end();)
	{
		ene = *IterationEnemy;
		if (pPlayer->IsCollidingWithAnim(**IterationEnemy)) {
			float eneX = ene->GetPositionX();
			float eneY = ene->GetPositionY();
			if (ene->IsCollidingWithAnim(*pPlayer))
			{
				SpawnExplosion(pPlayer->GetPositionX(), pPlayer->GetPositionY());
				sound.playSound(soundBlood, false);
				pPlayer->SetPositionX(32.0f);
				pPlayer->SetPositionY(41.0f);
				pPlayer->SetLives(pPlayer->GetLives() - 1);
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
			
			//Collision with Water
			for (IterationWaterTile = waterTileContainer.begin(); IterationWaterTile < waterTileContainer.end();)
			{
				wat = *IterationWaterTile;
				if (pPlayer->IsCollidingWithEnt(**IterationWaterTile)) 
				{
					pPlayer->SetWalkableWater(true);
					if (mask == 'S' && pPlayer->GetWalkableWater())
					{

					}
					else if (pPlayer->IsCollidingWithEnt(**IterationWaterTile))
					{
						SpawnExplosion(pPlayer->GetPositionX(), pPlayer->GetPositionY());
						sound.playSound(soundBlood, false);
						pPlayer->SetPositionX(32.0f);
						pPlayer->SetPositionY(41.0f);
						pPlayer->SetLives(pPlayer->GetLives() - 1);
						if (pPlayer->GetLives() == 0)
							pPlayer->SetDead(true);
					}
				}
				else
				{
					IterationWaterTile++;
				}
			}
			pPlayer->SetWalkableWater(false);

			//Collision with Lava
			for (IterationLavaTile = lavaTileContainer.begin(); IterationLavaTile < lavaTileContainer.end();)
			{
				lav = *IterationLavaTile;
					if (pPlayer->IsCollidingWithEnt(**IterationLavaTile))
					{
						SpawnExplosion(pPlayer->GetPositionX(), pPlayer->GetPositionY());
						sound.playSound(soundBlood, false);
						pPlayer->SetPositionX(32.0f);
						pPlayer->SetPositionY(41.0f);;
						pPlayer->SetLives(pPlayer->GetLives() - 1);
						if (pPlayer->GetLives() == 0)
							pPlayer->SetDead(true);
					}
				else
				{
					IterationLavaTile++;
				}
			}

			//Collision with wall
			for (IterationWall = wallContainer.begin(); IterationWall < wallContainer.end();)
			{
				wal = *IterationWall;
				if (pPlayer->IsCollidingWithEnt(**IterationWall))
				{
						pPlayer->SetPositionX(oldPositionX);
						pPlayer->SetPositionY(oldPositionY);
				}
				else
				{
					IterationWall++;
				}
			}

			//Collision with NarrowWall
			for (IterationNarrow = NarrowTileContainer.begin(); IterationNarrow < NarrowTileContainer.end();)
			{
				nar = *IterationNarrow;
				if (pPlayer->IsCollidingWithEnt(**IterationNarrow))
				{
					pPlayer->SetWalkableNarrow(true);
					if (mask == 'C' && pPlayer->GetWalkableNarrow())
					{

					}
					else if (pPlayer->IsCollidingWithEnt(**IterationNarrow))
					{
						if (direction == 'U')
						{
							pPlayer->SetPositionX(oldPositionX);
							pPlayer->SetPositionY(pPlayer->GetPositionY() + nar->GetSpriteWidth() / 2 - 15);
						}
						if (direction == 'D')
						{
							pPlayer->SetPositionX(oldPositionX);
							pPlayer->SetPositionY(pPlayer->GetPositionY() - nar->GetSpriteWidth() / 2 + 15);
						}
						if (direction == 'R')
						{
							pPlayer->SetPositionX(oldPositionX - nar->GetSpriteWidth() / 2 + 15);
							pPlayer->SetPositionY(pPlayer->GetPositionY());
						}
						if (direction == 'L')
						{
							pPlayer->SetPositionX(oldPositionX + nar->GetSpriteWidth() / 2 - 15);
							pPlayer->SetPositionY(pPlayer->GetPositionY());
						}
						else if (pPlayer->GetCollide() < -1.0f && pPlayer->GetMask() != 'C')
						{
							SpawnExplosion(pPlayer->GetPositionX(), pPlayer->GetPositionY());
							sound.playSound(soundBlood, false);
							pPlayer->SetPositionX(32.0f);
							pPlayer->SetPositionY(41.0f);
							pPlayer->SetLives(pPlayer->GetLives() - 1);
							if (pPlayer->GetLives() == 0)
								pPlayer->SetDead(true);
						}
					}
				}
				else
				{
					IterationNarrow++;
				}
			}
			pPlayer->SetWalkableNarrow(false);

			//Collision with Low Wall
			for (IterationLowWall = LowWallTileContainer.begin(); IterationLowWall < LowWallTileContainer.end();)
			{
				low = *IterationLowWall;
				if (pPlayer->IsCollidingWithEnt(**IterationLowWall))
				{

					pPlayer->SetWalkableLow(true);
					if (pPlayer->GetWalkableLow())
					{
						float oldoldX = oldPositionX;
						float oldoldY = oldPositionY;
					}
					if (mask == 'B' && pPlayer->GetWalkableLow())
					{

					}
					else
					{
						if (direction == 'U')
						{
							pPlayer->SetPositionX(oldPositionX);
							pPlayer->SetPositionY(pPlayer->GetPositionY() + low->GetSpriteWidth() / 2 - 15);
						}
						if (direction == 'D')
						{
							pPlayer->SetPositionX(oldPositionX);
							pPlayer->SetPositionY(pPlayer->GetPositionY() - low->GetSpriteWidth() / 2 + 15);
						}
						if (direction == 'R')
						{
							pPlayer->SetPositionX(oldPositionX - low->GetSpriteWidth() / 2 + 15);
							pPlayer->SetPositionY(pPlayer->GetPositionY());
						}
						if (direction == 'L')
						{
							pPlayer->SetPositionX(oldPositionX + low->GetSpriteWidth()/2 -15);
							pPlayer->SetPositionY(pPlayer->GetPositionY());
						}
						else if (pPlayer->GetCollide() < -1.0f && pPlayer->GetMask() != 'B')
						{
							SpawnExplosion(pPlayer->GetPositionX(), pPlayer->GetPositionY());
							sound.playSound(soundBlood, false);
							pPlayer->SetPositionX(32.0f);
							pPlayer->SetPositionY(41.0f);
							pPlayer->SetLives(pPlayer->GetLives() - 1);
							if (pPlayer->GetLives() == 0)
								pPlayer->SetDead(true);
						}
					}

				}
				else
				{
					IterationLowWall++;
				}
			}
			pPlayer->SetWalkableLow(false);


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


	//Enemies Move Down
	for (size_t i = 0; i < enemyContainer.size(); i++)
	{
		if (enemyContainer[i]->GetPositionY() > 600 - 33)
		{
			enemyContainer[i]->SetVerticalVelocity(-1.0f);
			enemyContainer[i]->SetYPos(144);
		}
		//Enemies Move Up
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

	int j = 0;
	for (IterationTile = tileContainer.begin(); IterationTile < tileContainer.end(); IterationTile++, j++)
	{
		tileContainer[j]->Draw(backBuffer);
	}

	int x = 0;
	for (IterationWaterTile = waterTileContainer.begin(); IterationWaterTile < waterTileContainer.end(); IterationWaterTile++, x++)
	{
		waterTileContainer[x]->Draw(backBuffer);
	}

	int l = 0;
	for (IterationLavaTile = lavaTileContainer.begin(); IterationLavaTile < lavaTileContainer.end(); IterationLavaTile++, l++)
	{
		lavaTileContainer[l]->Draw(backBuffer);
	}

	int w = 0;
	for (IterationWall = wallContainer.begin(); IterationWall < wallContainer.end(); IterationWall++, w++)
	{
		wallContainer[w]->Draw(backBuffer);
	}

	int n = 0;
	for (IterationNarrow = NarrowTileContainer.begin(); IterationNarrow < NarrowTileContainer.end(); IterationNarrow++, n++)
	{
		NarrowTileContainer[n]->Draw(backBuffer);
	}

	int z = 0;
	for (IterationLowWall = LowWallTileContainer.begin(); IterationLowWall < LowWallTileContainer.end(); IterationLowWall++, z++)
	{
		LowWallTileContainer[z]->Draw(backBuffer);
	}

	// W02.2: Draw all enemy aliens in container...
	int i = 0;
	for (IterationEnemy = enemyContainer.begin(); IterationEnemy < enemyContainer.end(); IterationEnemy++, i++)
	{
		enemyContainer[i]->Draw(backBuffer);
	}
	
	for (size_t i = 0; i < explosionContainer.size(); i++) {
		explosionContainer[i]->Draw(backBuffer);
	}

	SDL_Color colour = { 255, 0, 0, 255 };
	char fps[5];
	sprintf(fps, "%d", m_FPS);
	m_pBackBuffer->DrawText(colour, "Chiller.ttf", "FPS :", 25, 920, 5);
	m_pBackBuffer->DrawText(colour, "Chiller.ttf", fps , 25, 960, 5);

	// W02.1: Draw the player ship...
	if (pPlayer->IsDead() == false)
	pPlayer->Draw(backBuffer);
	else
	{
		SDL_Color colour = { 255, 0, 0, 255 };
		m_pBackBuffer->DrawText(colour, "Chiller.ttf", "Game Over", 100, 350,250);
	}

	backBuffer.Present();
}

void 
Game::Quit()
{
	m_looping = false;
}

void
Game::MovePlayerUp(char u)
{
	const Value& YPosData = Parser::GetInstance().document["YPos"];
	pPlayer->StartAnimation();
	switch (mask)
	{
	case 'N' :
		pPlayer->SetVerticalVelocity(-(YPosData["speed"].GetFloat()));
		playerSprite->SetYPos(YPosData["normalUp"].GetInt());
		break;
	case 'B':
		pPlayer->SetVerticalVelocity(-YPosData["speed"].GetFloat());
		pBatSprite->SetYPos(YPosData["batUp"].GetInt());
		break;
	case 'C':
		pPlayer->SetVerticalVelocity(-YPosData["speed"].GetFloat());
		pCatSprite->SetYPos(YPosData["catUp"].GetInt());
		break;
	case 'W':
		pPlayer->SetVerticalVelocity(-YPosData["wolfspeed"].GetFloat());
		pWolfSprite->SetYPos(YPosData["wolfUp"].GetInt());
		break;
	case 'S':
		pPlayer->SetVerticalVelocity(-YPosData["speed"].GetFloat());
		pSealSprite->SetYPos(YPosData["sealUp"].GetInt());
		break;
	}
	direction = u;
}

void
Game::MovePlayerDown(char u)
{
	const Value& YPosData = Parser::GetInstance().document["YPos"];
	pPlayer->StartAnimation();
	switch (mask)
	{
	case 'N' :
		pPlayer->SetVerticalVelocity(YPosData["speed"].GetFloat());
		playerSprite->SetYPos(YPosData["normalDown"].GetInt());
		break;
	case 'B' :
		pPlayer->SetVerticalVelocity(YPosData["speed"].GetFloat());
		pBatSprite->SetYPos(YPosData["batDown"].GetInt());
		break;
	case 'C':
		pPlayer->SetVerticalVelocity(YPosData["speed"].GetFloat());
		pCatSprite->SetYPos(YPosData["catDown"].GetInt());
		break;
	case 'W':
		pPlayer->SetVerticalVelocity(YPosData["wolfspeed"].GetFloat());
		pWolfSprite->SetYPos(YPosData["wolfDown"].GetInt());
		break;
	case 'S':
		pPlayer->SetVerticalVelocity(YPosData["speed"].GetFloat());
		pSealSprite->SetYPos(YPosData["sealDown"].GetInt());
		break;
	}
	direction = u;
}

void
Game::MovePlayerLeft(char u)
{
	const Value& YPosData = Parser::GetInstance().document["YPos"];
	pPlayer->StartAnimation();
	switch (mask)
	{
	case 'N' :
		pPlayer->SetHorizontalVelocity(-(YPosData["speed"].GetFloat()));
		playerSprite->SetYPos(YPosData["normalLeft"].GetInt());
		break;
	case 'B' :
		pPlayer->SetHorizontalVelocity(-YPosData["speed"].GetFloat());
		pBatSprite->SetYPos(YPosData["batLeft"].GetInt());
		break;
	case 'C':
		pPlayer->SetHorizontalVelocity(-YPosData["speed"].GetFloat());
		pCatSprite->SetYPos(YPosData["catLeft"].GetInt());
		break;
	case 'W':
		pPlayer->SetHorizontalVelocity(-YPosData["wolfspeed"].GetFloat());
		pWolfSprite->SetYPos(YPosData["wolfLeft"].GetInt());
		break;
	case 'S':
		pPlayer->SetHorizontalVelocity(-YPosData["speed"].GetFloat());
		pSealSprite->SetYPos(YPosData["sealLeft"].GetInt());
		break;
	}
	direction = u;
}

void
Game::MovePlayerRight(char u)
{
	const Value& YPosData = Parser::GetInstance().document["YPos"];
	pPlayer->StartAnimation();
	switch (mask)
	{
	case 'N':
		pPlayer->SetHorizontalVelocity(YPosData["speed"].GetFloat());
		playerSprite->SetYPos(YPosData["normalRight"].GetInt());
		break;
	case 'B':
		pPlayer->SetHorizontalVelocity(YPosData["speed"].GetFloat());
		pBatSprite->SetYPos(YPosData["batRight"].GetInt());
		break;
	case 'C':
		pPlayer->SetHorizontalVelocity(YPosData["speed"].GetFloat());
		pCatSprite->SetYPos(YPosData["catRight"].GetInt());
		break;
	case 'W':
		pPlayer->SetHorizontalVelocity(YPosData["wolfspeed"].GetFloat());
		pWolfSprite->SetYPos(YPosData["wolfRight"].GetInt());
		break;
	case 'S':
		pPlayer->SetHorizontalVelocity(YPosData["speed"].GetFloat());
		pSealSprite->SetYPos(YPosData["sealRight"].GetInt());
		break;
	}
	direction = u;
}

void 
Game::StopHorizontalMovement()
{
	pPlayer->SetHorizontalVelocity(0.0f);
	PauseAnimation();
}

void
Game::StopVerticalMovement()
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
	const Value& batData = Parser::GetInstance().document["bat"];
	pBatSprite = m_pBackBuffer->CreateAnimSprite(batData["sprite_location"].GetString());
	pPlayer->SetMask(toChar(batData["Mask"].GetString()));
	pPlayer->Initialise(pBatSprite);
	pPlayer->PauseAnimatedSprite();
	pBatSprite->SetFrameSpeed(batData["frame_speed"].GetFloat());
	pBatSprite->SetFrameWidth(batData["frame_width"].GetInt());
	pBatSprite->SetFrameHeight(batData["frame_height"].GetInt());
	pBatSprite->SetNumOfFrames(batData["num_frames"].GetInt());
	pBatSprite->SetYPos(batData["YPosition"].GetInt());
	mask = toChar(batData["Mask"].GetString());
	pBatSprite->SetLooping(batData["looping"].GetBool());
}

void
Game::CatForm()
{
	const Value& catData = Parser::GetInstance().document["cat"];
	pCatSprite = m_pBackBuffer->CreateAnimSprite(catData["sprite_location"].GetString());
	pPlayer->SetMask(toChar(catData["Mask"].GetString()));
	pPlayer->Initialise(pCatSprite);
	pPlayer->PauseAnimatedSprite();
	pCatSprite->SetFrameSpeed(catData["frame_speed"].GetFloat());
	pCatSprite->SetFrameWidth(catData["frame_width"].GetInt());
	pCatSprite->SetFrameHeight(catData["frame_height"].GetInt());
	pCatSprite->SetNumOfFrames(catData["num_frames"].GetInt());
	pCatSprite->SetYPos(catData["YPosition"].GetInt());
	mask = toChar(catData["Mask"].GetString());
	pCatSprite->SetLooping(catData["looping"].GetBool());
}

void
Game::WolfForm()
{
	const Value& wolfData = Parser::GetInstance().document["wolf"];
	pWolfSprite = m_pBackBuffer->CreateAnimSprite(wolfData["sprite_location"].GetString());
	pPlayer->SetMask(toChar(wolfData["Mask"].GetString()));
	pPlayer->Initialise(pWolfSprite);
	pPlayer->PauseAnimatedSprite();
	pWolfSprite->SetFrameSpeed(wolfData["frame_speed"].GetFloat());
	pWolfSprite->SetFrameWidth(wolfData["frame_width"].GetInt());
	pWolfSprite->SetFrameHeight(wolfData["frame_height"].GetInt());
	pWolfSprite->SetNumOfFrames(wolfData["num_frames"].GetInt());
	pWolfSprite->SetYPos(wolfData["YPosition"].GetInt());
	mask = toChar(wolfData["Mask"].GetString());
	pWolfSprite->SetLooping(wolfData["looping"].GetBool());
}

void
Game::SealForm()
{
	const Value& sealData = Parser::GetInstance().document["seal"];
	pSealSprite = m_pBackBuffer->CreateAnimSprite(sealData["sprite_location"].GetString());
	pPlayer->SetMask(toChar(sealData["Mask"].GetString()));
	pPlayer->Initialise(pSealSprite);
	pPlayer->PauseAnimatedSprite();
	pSealSprite->SetFrameSpeed(sealData["frame_speed"].GetFloat());
	pSealSprite->SetFrameWidth(sealData["frame_width"].GetInt());
	pSealSprite->SetFrameHeight(sealData["frame_height"].GetInt());
	pSealSprite->SetNumOfFrames(sealData["num_frames"].GetInt());
	pSealSprite->SetYPos(sealData["YPosition"].GetInt());
	mask = toChar(sealData["Mask"].GetString());
	pSealSprite->SetLooping(sealData["looping"].GetBool());
}

void
Game::HumanForm()
{
	const Value& playerData = Parser::GetInstance().document["player"];
	pPlayer->SetMask(toChar(playerData["Mask"].GetString()));
	pPlayer->Initialise(playerSprite);
	pPlayer->PauseAnimatedSprite();
	playerSprite->SetFrameSpeed(playerData["frame_speed"].GetFloat());
	playerSprite->SetFrameWidth(playerData["frame_width"].GetInt());
	playerSprite->SetFrameHeight(playerData["frame_height"].GetInt());
	playerSprite->SetNumOfFrames(playerData["num_frames"].GetInt());
	playerSprite->SetYPos(playerData["YPosition"].GetInt());
	mask = toChar(playerData["Mask"].GetString());
}

void
Game::CreateTile(float x, float y)
{
	// W02.2: Load the alien enemy sprite file.
	pTileSprite = m_pBackBuffer->CreateSprite("assets\\Grass.png");
	pTile = new Tiles();
	pTile->Initialise(pTileSprite);
	pTile->SetCharType('N');
	pTile->SetPositionX(x);
	pTile->SetPositionY(y);
	pTile->SetHorizontalVelocity(0.0f);
	pTile->SetVerticalVelocity(0.0f);
	pTile->SetDead(false);

	// W02.2: Add the new Enemy to the enemy container.
	tileContainer.push_back(pTile);
}

void
Game::CreateWaterTile(float x, float y)
{
	pTileWaterSprite = m_pBackBuffer->CreateSprite("assets\\Water.png");
	pWaterTile = new WaterTile();
	pWaterTile->Initialise(pTileWaterSprite);
	pWaterTile->SetCharType('W');
	pWaterTile->SetPositionX(x);
	pWaterTile->SetPositionY(y);
	pWaterTile->SetHorizontalVelocity(0.0f);
	pWaterTile->SetVerticalVelocity(0.0f);
	pWaterTile->SetDead(false);

	waterTileContainer.push_back(pWaterTile);
}

void
Game::CreateWall(float x, float y)
{
	pWallSprite = m_pBackBuffer->CreateSprite("assets\\Tree.png");
	pWall = new Tiles();
	pWall->Initialise(pWallSprite);
	pWall->SetCharType('B');
	pWall->SetPositionX(x);
	pWall->SetPositionY(y);
	pWall->SetHorizontalVelocity(0.0f);
	pWall->SetVerticalVelocity(0.0f);
	pWall->SetDead(false);

	wallContainer.push_back(pWall);
}

void
Game::CreateLavaTile(float x, float y)
{

	pLavaSprite = m_pBackBuffer->CreateSprite("assets\\Lava.png");
	pLava = new LavaTile();
	pLava->Initialise(pLavaSprite);
	pLava->SetCharType('L');
	pLava->SetPositionX(x);
	pLava->SetPositionY(y);
	pLava->SetHorizontalVelocity(0.0f);
	pLava->SetVerticalVelocity(0.0f);
	pLava->SetDead(false);

	lavaTileContainer.push_back(pLava);
}

void
Game::CreateNarrow(float x, float y)
{

	pNarrowSprite = m_pBackBuffer->CreateSprite("assets\\Narrow.png");
	pNarrow = new Narrow();
	pNarrow->Initialise(pNarrowSprite);
	pNarrow->SetCharType('L');
	pNarrow->SetPositionX(x);
	pNarrow->SetPositionY(y);
	pNarrow->SetHorizontalVelocity(0.0f);
	pNarrow->SetVerticalVelocity(0.0f);
	pNarrow->SetDead(false);

	NarrowTileContainer.push_back(pNarrow);
}

void
Game::CreateLowWall(float x, float y)
{
	pLowWallSprite = m_pBackBuffer->CreateSprite("assets\\low.png");
	pLow = new LowWall();
	pLow->Initialise(pLowWallSprite);
	pLow->SetCharType('Z');
	pLow->SetPositionX(x);
	pLow->SetPositionY(y);
	pLow->SetHorizontalVelocity(0.0f);
	pLow->SetVerticalVelocity(0.0f);
	pLow->SetDead(false);

	LowWallTileContainer.push_back(pLow);
}

char
Game::toChar(string temp)
{
		return temp.at(0);
}