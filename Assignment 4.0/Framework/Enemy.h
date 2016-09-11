	#ifndef ENEMY_H
	#define ENEMY_H
	#include "AnimatedEntity.h"

class Enemy : public AnimatedEntity
	{
	public:
		Enemy();
		void SetAlgorithm(char a);
		void AlgorithmA();
		void AlgorithmB();
		void AlgorithmC();
		void AlgorithmD();
		void AlgorithmE();
		void AlgorithmF();

		float getTileX(float x);
		float getTileY(float y);

		char algorithm;
	};
	#endif