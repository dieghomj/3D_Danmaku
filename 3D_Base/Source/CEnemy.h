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

	void SetTargetPos(D3DXVECTOR3 pos) { m_vTargetPos = pos; }

protected:

	enEnemyState m_EnemyState;

	D3DXVECTOR3 m_vTargetPos;
	float m_MoveSpeed;
	float m_TurnSpeed;

	std::random_device rd;

};
