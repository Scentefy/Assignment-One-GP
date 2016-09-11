// COMP710 GP 2D Framework
#ifndef __GAME_H__
#define __GAME_H__
#include <vector>
#include "SoundSys.h"
#include "Enemy.h"
#include "Blood.h"
#include "Tiles.h"
#include "WaterTile.h"
#include "Lava.h"
#include "Narrow.h"
#include "LowWall.h"

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

	void MovePlayerUp(char u);
	void MovePlayerDown(char u);
	void MovePlayerLeft(char u);
	void MovePlayerRight(char u);
	void StopHorizontalMovement();
	void StopVerticalMovement();
	void PauseAnimation();

	void SpawnEnemy(float x, float y);
	void SpawnExplosion(float x, float y);
	void BatForm();
	void CatForm();
	void WolfForm();
	void SealForm();
	void HumanForm();

	void CreateTile(float x, float y);
	void CreateWaterTile(float x, float y);
	void CreateWall(float x, float y);
	void CreateLavaTile(float x, float y);
	void CreateNarrow(float x, float y);
	void CreateLowWall(float x, float y);
	void CreateEndTile(float x, float y);

	char toChar(std::string temp);

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
	float oldDelta;
	float m_lag;
	float m_executionTime;
	int m_lastTime;
	int m_frameCount;
	int m_FPS;
	int m_numUpdates;
	bool m_drawDebugInfo;

	// Game Entities:
	char mask;
	char tile;
	char direction;

	//Containers
	std::vector<Enemy*> enemyContainer;
	std::vector<Blood*> explosionContainer;
	std::vector<Tiles*> tileContainer;
	std::vector<WaterTile*> waterTileContainer;
	std::vector<Tiles*> wallContainer;
	std::vector<LavaTile*> lavaTileContainer;
	std::vector<Narrow*> NarrowTileContainer;
	std::vector<LowWall*> LowWallTileContainer;

	Enemy* ene;
	Blood* exp;
	WaterTile* wat;
	Tiles* wal;
	LavaTile* lav;
	Narrow* nar;
	LowWall* low;

	//FMOD
	SoundSys sound;
	SoundClass soundBlood;

};

#endif // __GAME_H__
