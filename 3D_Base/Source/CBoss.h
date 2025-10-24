 #pragma once
#include "Global.h"
#include "CEnemy.h"
#include "CShot.h"
#include "CSprite3D.h"

class CBoss : public CEnemy
{
public:

	enum enAttackPattern
	{
		STANDBY = 0,
		ROTATING,
		WAYCROSS,
		WAVE,
	};

	CBoss();
	virtual ~CBoss();

	void Update() override;
	void Draw(D3DXMATRIX& View, D3DXMATRIX& Proj, LIGHT& Light, CAMERA& Camera) override;

	void IncCadenceTimer(float delta) { m_ShootTimer += delta; }
	void DecCadenceTimer(float delta) { m_ShootTimer -= delta; }

	float GetShootCd() const { return m_ShootCd; };
	int GetAttackPattern() const { return m_AttackPattern; }
	float GetCrossAngleStep() const { return m_angleStep; }

private:

	float	m_ShootCd		;
	float	m_ShootTimer	;
	float	m_PatternTimer	;
	float	m_angleStep		;
	int		m_AttackPattern ;
	int		m_PatternStep	;

};