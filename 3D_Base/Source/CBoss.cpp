#include "CBoss.h"

CBoss::CBoss()
	: m_ShootCd			(6.0f)
	, m_ShootTimer		(0.0f)
	, m_AttackPattern	(1)
	, m_PatternStep		(0)
	, m_PatternTimer	(0.0f)
	, m_angleStep		(0.0f)
	
{
	m_Health = 1000.f;
	m_MoveSpeed = 0.04f;
}



CBoss::~CBoss()
{

}

void CBoss::Update()
{
	D3DXVECTOR3 dir;
	float distance;
	GetTargetDir(&dir, &distance);

	if (m_EnemyState == CEnemy::DESPAWN)
	{
		m_AttackPattern = enAttackPattern::WAYCROSS;
	}

	if (m_Health <= 500.f)
	{
		m_AttackPattern = enAttackPattern::ROTATING;
	}

	if (m_EnemyState == CBoss::CHASING)
	{
		m_vPosition.x += dir.x * m_MoveSpeed;
		m_vPosition.z += dir.z * m_MoveSpeed;

		if (distance <= 150.f)
			m_EnemyState = CEnemy::ATTACKING;	

	}

	float angle = 0.1f * PI / 2;

	m_Shot = false;

	if (m_EnemyState == enEnemyState::ATTACKING)
	{

		switch (m_AttackPattern)
		{
		case enAttackPattern::ROTATING:
			SetRotation(0.f, m_angleStep * angle, 0.f);
			m_angleStep++;

			m_Shot = true;
			break;
		case enAttackPattern::WAYCROSS:
			m_EnemyState = CEnemy::CHASING;
			m_Shot = true;
			break;
		}

		if (distance >= 200.f)
			m_EnemyState = CEnemy::CHASING;
	}


}

void CBoss::Draw(D3DXMATRIX& View, D3DXMATRIX& Proj, LIGHT& Light, CAMERA& Camera)
{

	CEnemy::Draw(View, Proj, Light, Camera);

}


