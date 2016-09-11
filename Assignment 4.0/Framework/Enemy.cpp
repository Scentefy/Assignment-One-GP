#include "Enemy.h"
#include "Parser.h"
using namespace std;

Enemy::Enemy()
	:algorithm(0)
{
}
// Spawn, Move Distance, Move back
void
Enemy::SetAlgorithm(char a)
{
	algorithm = a;
	if (algorithm == 'A')
	{
		AlgorithmA();
	}
	if (algorithm == 'B')
	{
		AlgorithmB();
	}
}

void
Enemy::AlgorithmA()
{
	const Value& enemyData = Parser::GetInstance().document["YPos"];
		if (GetPositionX() == 57.0f && GetPositionY() == 105.0f)
		{
			//WalkingRight
			this->SetHorizontalVelocity(2.5f);
			this->SetYPos(enemyData["enemyRight"].GetInt());
		}
		if (GetPositionX() == 169.0f && GetPositionY() == 105.0f)
		{
			this->SetHorizontalVelocity(-2.5f);
			this->SetYPos(enemyData["enemyLeft"].GetInt());
		}
}

void
Enemy::AlgorithmB()
{
	const Value& enemyData = Parser::GetInstance().document["YPos"];
	if (GetPositionX() == 57.0f && GetPositionY() == 105.0f)
	{
		//WalkingRight
		this->SetHorizontalVelocity(2.0f);
		this->SetYPos(enemyData["enemyRight"].GetInt());
	}
	if (GetPositionX() == 169.0f && GetPositionY() == 201.0f)
	{
		this->SetHorizontalVelocity(-2.0f);
		this->SetYPos(enemyData["enemyLeft"].GetInt());
	}
}

float
Enemy::getTileX(float x)
{
	return 32.0f + 32.0f * x;
}

float
Enemy::getTileY(float y)
{
	return 41.0f + 32.0f * y;
}

//for (size_t i = 0; i < enemyContainer.size(); i++)
//{
//	if (enemyContainer[i]->GetPositionY() > 600 - 33)
//	{
//		enemyContainer[i]->SetVerticalVelocity(-1.0f);
//		enemyContainer[i]->SetYPos(YPosData["enemyUp"].GetInt());
//	}
//	//Enemies Move Down
//	else if (enemyContainer[i]->GetPositionY() < 33)
//	{
//		enemyContainer[i]->SetVerticalVelocity(1.0f);
//		enemyContainer[i]->SetYPos(YPosData["enemyDown"].GetInt());
//	}
//}