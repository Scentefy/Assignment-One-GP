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

	void MovePlayer(std::string direction);
	void StopSpaceShipMovement();
	void FireSpaceShipBullet();

	void SpawnEnemy(float x, float y);
	void SpawnExplosion(int x, int y);
	void BatForm();
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
	enum Mask { None, Bat, Lion, Mouse, Seal, Chicken };
	char mask;
	//Background Sprite   
	Sprite* backGround;
	//Sprites
	AnimatedSprite* playerSprite;
	AnimatedSprite* enemySprite;
	AnimatedSprite* pBatSprite;
	// W02.2: Add an alien enemy container field.
	std::vector<AnimatedEntity*> enemyContainer;
	// W02.3: Add a bullet container field.
	std::vector<Bullet*> bulletContainer;
	
	std::vector<AnimatedEntity*> explosionContainer;

	Bullet* bull;
	AnimatedEntity* ene;
	AnimatedEntity* exp;

	std::vector<AnimatedEntity*>::iterator iter;
	std::vector <AnimatedEntity*>::iterator itere;

	//FMO
	SoundSys sound;
	SoundClass soundBlood;

private:

};

#endif // __GAME_H__
