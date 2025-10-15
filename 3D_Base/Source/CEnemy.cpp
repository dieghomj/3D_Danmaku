#include "CEnemy.h"
#include <random>

CEnemy::CEnemy()
	: m_vTargetPos	( D3DXVECTOR3() )
	, m_EnemyState	( enEnemyState::DESPAWN )
	, m_MoveSpeed	( 0.05f )
	, m_TurnSpeed	( 0.05f )
{

}

CEnemy::~CEnemy()
{
}

void CEnemy::Respawn()
{
	m_vPosition = D3DXVECTOR3(0.f, -10.f, 0.f);
}

void CEnemy::Update()
{
	std::mt19937 gen(rd());
	std::uniform_real_distribution<float> dis(0.1f, 1.1f);
	float rng = dis(gen);
	//Target player
	//Make the enemy spawn randomly from outside the screen 
	//Make normal enemies that come to you 
	//Make elites 
	//Make random wave enemies
	srand(static_cast<float>(time(0))); // ÉVÅ[ÉhÇèâä˙âª
	int randomNumber = rand()%10;

	D3DXVECTOR3 dir;
	float distance;
	D3DXVec3Subtract(&dir,&m_vTargetPos, &m_vPosition);
	distance = D3DXVec3LengthSq(&dir);
	D3DXVec3Normalize(&dir,&dir);

	if (distance <= 100.f && distance >= 20.f)
	{
		m_vPosition.y = 0.5f;
		m_EnemyState = enEnemyState::CHASING;
	}
	else
		m_EnemyState = m_EnemyState == enEnemyState::CHASING ? m_EnemyState : enEnemyState::DESPAWN;


	if (m_EnemyState == enEnemyState::CHASING)
	{
		m_vPosition.x += dir.x * m_MoveSpeed * rng;
		m_vPosition.y += dir.y * m_MoveSpeed * rng;
		m_vPosition.z += dir.z * m_MoveSpeed * rng;
	}
	if (distance <= 0.1f)
		Respawn();

}
