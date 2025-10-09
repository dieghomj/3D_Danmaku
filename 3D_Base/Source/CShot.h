#pragma once

#include "CSpriteObject.h"

/**************************************************
*	�e�N���X
**/
class CShot
	: public CSpriteObject
{
public:



public:
	CShot();
	virtual ~CShot() override;

	virtual void Update() override;
	virtual void Draw(D3DXMATRIX& View, D3DXMATRIX& Proj) override;

	//�e���Đݒ�
	void Reload(const D3DXVECTOR3& Pos, float RotY);

	void SetDisplay(bool disp) { m_Display = disp; }

	virtual bool IsHit(CGameObject* obj, float rad);

	float GetCadence() const { return m_Cadence; }

protected:
	bool		m_Display;			//�\���ؑ�
	int			m_DisplayTime;		//�\�����鎞��
	
	D3DXVECTOR3	m_MoveDirection;	//�ړ�����
	float		m_MoveSpeed;		//�ړ����x
	float		m_Cadence;			//�A�ˑ��x
	
};
