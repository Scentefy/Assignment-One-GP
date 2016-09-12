	#ifndef ENEMY_H
	#define ENEMY_H
	#include "AnimatedEntity.h"

class Enemy : public AnimatedEntity
	{
	public:
		Enemy();
		char GetAlgorithm();
		void SetAlgorithm(char a);
		void StartAlgorithm();

		char GetDirection();
		void SetDirection(char d);

		float GetMaxDistance();
		void SetMaxDistance(float m);

		float GetIniX();
		void SetIniX(float x);

		float GetIniY();
		void SetIniY(float y);

		void AlgorithmPatrolDirection();
		void AlgorithmB();

		float getTileX(float x);
		float getTileY(float y);

		void SetPatrol(float maxd, float maxr, float maxu, float maxl);

		char m_algorithm;
		char m_direction;
		float m_maxDistance;
		float m_iniX;
		float m_iniY;
		float maxdown, maxright, maxup, maxleft;
	};
	#endif