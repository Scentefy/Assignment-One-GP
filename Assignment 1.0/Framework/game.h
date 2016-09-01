// COMP710 GP 2D Framework
#ifndef __GAME_H__
#define __GAME_H__
#include "Enemy.h"
#include <vector>
#include "Bullet.h"
#include "AnimatedEntity.h"
#include "SoundSys.h"

// Forward Declarations
class BackBuffer;
class InputHandler;
class Sprite;
class AnimatedSprite;
class SoundSys;

class Game
{
	//Member Methods:
public:
	static Game& GetInstance();
	static void DestroyInstance();
	~Game();

	bool Initialise();
	bool DoGameLoop();
	void Quit();

	void MovePlayerUp();
	void MovePlayerDown();
	void MovePlayerLeft();
	void MovePlayerRight();
	void StopSpaceShipMovementHorizontal();
	void StopSpaceShipMovementVertical();
	void FireSpaceShipBullet();

	void SpawnEnemy(float x, float y);
	void SpawnExplosion(int x, int y);
	void BatForm();
	void CatForm();
	void LionForm();
	void BearForm();
	void WolfForm();
	void SealForm();
	void HumanForm();
	
protected:
	void Process(float deltaTime);
	void Draw(BackBuffer& backBuffer);

private:
	Game(const Game& game);
	Game& operator=(const Game& game);
	
	Game();

	//Member Data:
public:

protected:
	static Game* sm_pInstance;
	BackBuffer* m_pBackBuffer;
	InputHandler* m_pInputHandler;
	bool m_looping;

	// Simulation Counters:
	float m_elapsedSeconds;
	float m_lag;
	float m_executionTime;
	int m_lastTime;
	int m_frameCount;
	int m_FPS;
	int m_numUpdates;
	bool m_drawDebugInfo;

	// Game Entities:
	char mask;
	//Background Sprite   
	Sprite* backGround;
	//Sprites
	AnimatedSprite* playerSprite;
	AnimatedSprite* enemySprite;
	AnimatedSprite* pBatSprite;
	AnimatedSprite* pCatSprite;
	AnimatedSprite* pLionSprite;
	AnimatedSprite* pBearSprite;
	AnimatedSprite* pWolfSprite;
	AnimatedSprite* pSealSprite;
	// W02.2: Add an alien enemy container field.
	std::vector<AnimatedEntity*> enemyContainer;
	// W02.3: Add a bullet container field.
	std::vector<Bullet*> bulletContainer;
	
	std::vector<AnimatedEntity*> explosionContainer;

	Bullet* bull;
	AnimatedEntity* ene;
	AnimatedEntity* exp;

	//Deletion Iterators
	std::vector<AnimatedEntity*>::iterator IterationEnemy;
	std::vector <AnimatedEntity*>::iterator IterationDeath;

	//FMO
	SoundSys sound;
	SoundClass soundBlood;

private:

};

#endif // __GAME_H__
