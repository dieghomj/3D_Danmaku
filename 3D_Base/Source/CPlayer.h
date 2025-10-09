#pragma once

#include "CCharacter.h"

/**************************************************
*	�v���C���[�N���X.
**/
class CPlayer
	: public CCharacter	//�L�����N�^�[�N���X���p��.
{
public:

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

	int GetShotNumber() const { return m_ShotNumber; }
	int GetShotType()	const { return m_ShotType; }

protected:

	int			 m_shotMax;
	float		 m_TurnSpeed;	//��]���x
	float		 m_MoveSpeed;	//�ړ����x
	enMoveState	 m_MoveState;	//�ړ����
	enShotNumber m_ShotNumber;
	enShotType	 m_ShotType;
};