#pragma once

#include "CCharacter.h"

/**************************************************
*	�v���C���[�N���X.
**/
class CPlayer
	: public CCharacter	//�L�����N�^�[�N���X���p��.
{
public:

	enum enShotNumber
	{
		Single = 0,
		Double,
		Triple,
	};

	enum enShotType
	{
		Simple = 0,
		Rapid,
		Homing,
		Laser,
	};

	//�ړ���ԗ񋓌^
	enum enMoveState
	{
		Stop = 0,	//��~
		Forward,	//�O�i
		Backward,	//���
		Leftward,
		Rightward,
		TurnLeft,	//����]
		TurnRight,	//�E��]
	};
public:
	CPlayer();
	virtual ~CPlayer() override;

	virtual void Update() override;
	virtual void Draw(
		D3DXMATRIX& View, D3DXMATRIX& Proj, LIGHT& Light, CAMERA& Camera ) override;

	//���W�R������
	void RadioControl();

protected:

	int			m_shotMax;
	float		m_TurnSpeed;	//��]���x
	float		m_MoveSpeed;	//�ړ����x
	enMoveState	m_MoveState;	//�ړ����
};