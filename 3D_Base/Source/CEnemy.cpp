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
	m_EnemyState = enEnemyState::DESPAWN;
	std::mt19937 gen(rd());
	std::uniform_real_distribution<float> dis(-1.0f, 1.0f);
	float rng = dis(gen);
	m_vPosition.x = m_vTargetPos.x + 150.0 * rng;
	rng = dis(gen);
	m_vPosition.z = m_vTargetPos.z + 150.0 * rng;
	m_vPosition.y = -10.f;
	m_Health = MAX_HEALTH;
}

void CEnemy::Update()
{
	//Target player
	//Make the enemy spawn randomly from outside the screen 
	//Make normal enemies that come to you 
	//Make elites 
	//Make random wave enemies

	if (m_Health < 0.0f)
	{
		Respawn();
	}

	D3DXVECTOR3 dir;
	float distance;
	
	GetTargetDir(&dir, &distance);

	if (distance >= 50.f)
	{
		m_vPosition.y = 1.0f;
		m_EnemyState = enEnemyState::CHASING;
	}

	if (m_EnemyState == enEnemyState::CHASING)
	{
		m_vPosition.x += dir.x * m_MoveSpeed;
		m_vPosition.y += dir.y * m_MoveSpeed;
		m_vPosition.z += dir.z * m_MoveSpeed;
	}
	float offset = 0.5f;
	if (m_EnemyState == enEnemyState::SEARCHING)
	{
		std::mt19937 gen(rd());
		std::uniform_real_distribution<float> dis(-1.0f, 1.0f);
		float rng = dis(gen);
		m_vPosition.x += dir.x * m_MoveSpeed + offset * rng;
		m_vPosition.z += dir.z * m_MoveSpeed + offset * rng;
	}

	if (distance <= 1.0f)
		m_IsInTarget = true;
}

void CEnemy::Draw(D3DXMATRIX& View, D3DXMATRIX& Proj, LIGHT& Light, CAMERA& Camera)
{
	CCharacter::Draw(View, Proj, Light, Camera);
}

void CEnemy::GetTargetDir(D3DXVECTOR3* dir, float* distance)
{
	D3DXVec3Subtract(dir, &m_vTargetPos, &m_vPosition);
	*distance = D3DXVec3LengthSq(dir);
	D3DXVec3Normalize(dir, dir);
}
