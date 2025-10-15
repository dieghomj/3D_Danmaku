#pragma once

#include <random>
#include <stdio.h>
#include "CCharacter.h"

/**************************************************
*	エネミークラス
**/
class CEnemy
	: public CCharacter
{
private:
	float MAX_HEALTH = 10;
public:

	enum enEnemyState
	{
		DESPAWN,
		SPAWNING,
		CHASING,

	};

public:

	CEnemy();
	virtual ~CEnemy();

	virtual void Update() override;
	void Respawn();
	void Restart();

	void SetTargetPos(D3DXVECTOR3 pos) { m_vTargetPos = pos; }
	void SetDamagedValue(float dmg) { m_Health -= dmg; }

	enEnemyState GetEnemyState() const { return m_EnemyState; }


protected:

	enEnemyState m_EnemyState;

	float m_Health;

	D3DXVECTOR3 m_vTargetPos;
	float m_MoveSpeed;
	float m_TurnSpeed;

	std::random_device rd;

};
