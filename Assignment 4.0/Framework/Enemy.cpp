#include "Enemy.h"
#include "Parser.h"
using namespace std;

Enemy::Enemy()
	: m_algorithm(0)
	, m_maxDistance(0)
	, m_direction(0)
	, m_iniX(0)
	, m_iniY(0)
	, maxdown(0)
	, maxup(0)
	, maxleft(0)
	, maxright(0)
{
}

char
Enemy::GetAlgorithm()
{
	return m_algorithm;
}

void
Enemy::SetAlgorithm(char a)
{
	m_algorithm = a;
}

void
Enemy::StartAlgorithm()
{
	if (m_algorithm == 'A')
	{
		AlgorithmPatrolDirection();
	}
	if (m_algorithm == 'B')
	{
		AlgorithmB();
	}
}

char
Enemy::GetDirection()
{
	return m_direction;
}

void
Enemy::SetDirection(char d)
{
	m_direction = d;
}

float
Enemy::GetMaxDistance()
{
	return m_maxDistance;
}

void
Enemy::SetMaxDistance(float m)
{
	m_maxDistance = m;
}

float 
Enemy::GetIniX()
{
	return m_iniX;
}

void
Enemy::SetIniX(float x)
{
	m_iniX = x;
}

float
Enemy::GetIniY()
{
	return m_iniY;
}

void 
Enemy::SetIniY(float y)
{
	m_iniY = y;
}

void
Enemy::AlgorithmPatrolDirection()
{
	const Value& enemyData = Parser::GetInstance().document["YPos"];
	//Right Patrol
	if (m_direction == 'R')
	{
		if (GetPositionX() == getTileX(GetIniX()))
		{
			//WalkingRight
			this->SetHorizontalVelocity(enemyData["speed"].GetFloat());
			this->SetYPos(enemyData["enemyRight"].GetInt());
		}
		if (GetPositionX() > getTileX(GetIniX() + GetMaxDistance()))
		{
			this->SetHorizontalVelocity(-(enemyData["speed"].GetFloat()));
			this->SetYPos(enemyData["enemyLeft"].GetInt());
		}
	}

	//Left Patrol
	if (m_direction == 'L')
	{
		if (GetPositionX() == getTileX(GetIniX()))
		{
			//WalkingRight
			this->SetHorizontalVelocity(-(enemyData["speed"].GetFloat()));
			this->SetYPos(enemyData["enemyLeft"].GetInt());

		}
		if (GetPositionX() < getTileX(GetIniX() -GetMaxDistance()))
		{
			this->SetHorizontalVelocity(enemyData["speed"].GetFloat());
			this->SetYPos(enemyData["enemyRight"].GetInt());
			//WalkingLeft
		}
	}

	//Walking Up Patrol
	if (m_direction == 'U')
	{
		if (GetPositionY() == getTileY(GetIniY()))
		{
			//WalkingUp
			this->SetVerticalVelocity(-(enemyData["speed"].GetFloat()));
			this->SetYPos(enemyData["enemyUp"].GetInt());
		}
			//Walking Down
		if (GetPositionY() < getTileY(GetIniY() - (GetMaxDistance())))
		{
			this->SetVerticalVelocity(enemyData["speed"].GetFloat());
			this->SetYPos(enemyData["enemyDown"].GetInt());
		}
	}

	//Walking Down Patrol
	if (m_direction == 'D')
	{
		//WalkingDown
		if (GetPositionY() == getTileY(GetIniY()))
		{
			this->SetVerticalVelocity(enemyData["speed"].GetFloat());
			this->SetYPos(enemyData["enemyDown"].GetInt());
		}
		//Walking Down
		if (GetPositionY() > getTileY(GetIniY() + (GetMaxDistance())))
		{
			this->SetVerticalVelocity(-(enemyData["speed"].GetFloat()));
			this->SetYPos(enemyData["enemyUp"].GetInt());
		}
	}
}

void
Enemy::AlgorithmB()
{
	const Value& enemyData = Parser::GetInstance().document["YPos"];
	//Right Patrol
	if (m_direction == 'R')
	{
		if (GetPositionX() != getTileX(maxright))
		{
			//WalkingRight
			this->SetHorizontalVelocity(enemyData["speed"].GetFloat());
			this->SetYPos(enemyData["enemyRight"].GetInt());
		}
		if (GetPositionX() > getTileX(maxright))
		{
			m_direction = 'U';
			SetHorizontalVelocity(0.0f);
		}
	}

	//Left Patrol
	if (m_direction == 'L')
	{
		if (GetPositionX() != getTileX(maxleft))
		{
			//WalkingRight
			this->SetHorizontalVelocity(-(enemyData["speed"].GetFloat()));
			this->SetYPos(enemyData["enemyLeft"].GetInt());
		}
		if (GetPositionX() < getTileX(maxleft))
		{
			m_direction = 'D';
			SetHorizontalVelocity(0.0f);
		}
	}

	//Walking Up Patrol
	if (m_direction == 'U')
	{
		if (GetPositionY() != getTileY(maxup))
		{
			//WalkingUp
			this->SetVerticalVelocity(-(enemyData["speed"].GetFloat()));
			this->SetYPos(enemyData["enemyUp"].GetInt());
		}
		if (GetPositionY() < getTileY(maxup))
		{
			m_direction = 'L';
			SetVerticalVelocity(0.0f);
		}
	}

	//Walking Down Patrol
	if (m_direction == 'D')
	{
		//WalkingDown
		if (GetPositionY() != getTileY(maxdown))
		{
			this->SetVerticalVelocity(enemyData["speed"].GetFloat());
			this->SetYPos(enemyData["enemyDown"].GetInt());

			if (GetPositionY() > getTileY(maxdown))
			{
				m_direction = 'R';
				SetVerticalVelocity(0.0f);
			}
		}
	}
}

float
Enemy::getTileX(float x)
{
	// returns 0 x position on grid
	return 32.0f + 32.0f * x;
}

float
Enemy::getTileY(float y)
{
	// returns 0 y position on grid
	return 41.0f + 32.0f * y;
}

void Enemy::SetPatrol(float maxd, float maxr, float maxu, float maxl)
{
	maxdown = maxd;
	maxright = maxr;
	maxup = maxu;
	maxleft = maxl;
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