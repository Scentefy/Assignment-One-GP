// COMP710 GP 2D Framework
#ifndef __GAME_H__
#define __GAME_H__
#include <vector>
#include "AnimatedEntity.h"
#include "SoundSys.h"
#include "Enemy.h"
#include "Blood.h"
#include "Tiles.h"

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
	void PauseAnimation();

	void SpawnEnemy(float x, float y);
	void SpawnExplosion(float x, float y);
	void BatForm();
	void CatForm();
	void LionForm();
	void BearForm();
	void WolfForm();
	void SealForm();
	void HumanForm();

	void CreateTile(float x, float y);
	
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

	// W02.2: Add an alien enemy container field.
	std::vector<Enemy*> enemyContainer;
	
	std::vector<Blood*> explosionContainer;
	std::vector<Tiles*> tileContainer;

	Enemy* ene;
	Blood* exp;

	//Deletion Iterators
	std::vector<Enemy*>::iterator IterationEnemy;
	std::vector <Blood*>::iterator IterationDeath;
	std::vector <Tiles*>::iterator IterationTile;

	//FMO
	SoundSys sound;
	SoundClass soundBlood;

private:

};

#endif // __GAME_H__
