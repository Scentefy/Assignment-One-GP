#include "Enemy.h"
#include "Parser.h"
using namespace std;

Enemy::Enemy()
{

}

void
Enemy::AlgorithmA()
{
	const Value& enemyData = Parser::GetInstance().document["YPos"];
		if (GetPositionX() == 73.0f && GetPositionY() == 105.0f)
		{
			//WalkingRight
			this->SetHorizontalVelocity(2.0f);
			this->SetYPos(enemyData["enemyRight"].GetInt());
		}
		if (GetPositionX() == 169.0f && GetPositionY() == 105.0f)
		{
			this->SetHorizontalVelocity(-2.0f);
			this->SetYPos(enemyData["enemyLeft"].GetInt());
		}
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