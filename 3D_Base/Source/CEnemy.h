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
		SEARCHING,
		CHASING,
		ATTACKING,
	};

public:

	CEnemy();
	virtual ~CEnemy();

	void Update() override;
	void Draw(D3DXMATRIX& View, D3DXMATRIX& Proj, LIGHT& Light, CAMERA& Camera) override;

	void Respawn();

	void SetEnemyState(enEnemyState state) { m_EnemyState = state; }
	void SetTargetPos(D3DXVECTOR3 pos) { m_vTargetPos = pos; }
	void SetDamagedValue(float dmg) { m_Health -= dmg; }
	void SetHealth(float hp) { m_Health = hp; }

	bool IsInTarget() const { return m_IsInTarget; }

	float GetHealth() const { return m_Health; }
	enEnemyState GetEnemyState() const { return m_EnemyState; }

protected:
	void GetTargetDir(D3DXVECTOR3* dir, float* distance);

protected:
	bool m_IsInTarget;

	enEnemyState m_EnemyState;

	float m_Health;

	D3DXVECTOR3 m_vTargetPos;
	float m_MoveSpeed;
	float m_TurnSpeed;

	std::random_device rd;

};
