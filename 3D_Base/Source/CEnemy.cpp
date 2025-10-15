#include "CEnemy.h"
#include <random>

CEnemy::CEnemy()
	: m_vTargetPos	( D3DXVECTOR3() )
	, m_EnemyState	( enEnemyState::DESPAWN )
	, m_MoveSpeed	( 0.05f )
	, m_TurnSpeed	( 0.05f )
	, m_Health		( 10.0f )
{

}

CEnemy::~CEnemy()
{

}

void CEnemy::Respawn()
{
	std::mt19937 gen(rd());
	std::uniform_real_distribution<float> dis(-1.0f, 1.0f);
	float rng = dis(gen);
	m_vPosition.x = m_vTargetPos.x + 400.0 * rng / 1.0f;
	m_vPosition.z = m_vTargetPos.z + 400.0 * rng / 1.0f;
	m_vPosition.y = -10.f;
	m_Health = MAX_HEALTH;
}

void CEnemy::Update()
{

	if (m_Health < 0.0f)
	{
		Respawn();
	}

	std::mt19937 gen(rd());
	std::uniform_real_distribution<float> dis(0.0, 1.0f);
	float rng = dis(gen);
	//Target player
	//Make the enemy spawn randomly from outside the screen 
	//Make normal enemies that come to you 
	//Make elites 
	//Make random wave enemies
	srand(static_cast<float>(time(0))); // ƒV[ƒh‚ð‰Šú‰»
	int randomNumber = rand()%10;

	D3DXVECTOR3 dir;
	float distance;
	D3DXVec3Subtract(&dir,&m_vTargetPos, &m_vPosition);
	distance = D3DXVec3LengthSq(&dir);
	D3DXVec3Normalize(&dir,&dir);

	if (distance >= 50.f)
	{
		m_vPosition.y = 1.0f;
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
