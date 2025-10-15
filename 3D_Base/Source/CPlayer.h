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

	bool IsBomb() const { return m_IsBomb; }

	int GetShotNumber() const { return m_ShotNumber; }
	int GetShotType()	const { return m_ShotType; }
	float GetShotCadence() const { return m_Cadence; }
	float GetNWaySpreadDeg() const { return m_NWaySrpeadDeg; }
	float GetChargedShotTime() const { return m_ChargedShotTime; }
	float GetChargedShotMax() const { return m_ChargedShotMax; }

	void IncCadenceTimer(float delta) { m_CadenceTimer += delta; }
	void DecCadenceTimer(float delta) { m_CadenceTimer -= delta; }
	void ResetCadenceTimer() { m_CadenceTimer = m_Cadence; }

	void SetPlayerCadence(float cad) { m_Cadence = cad; }
	void SetPlayerShotType(enShotType type) { m_ShotType = type; }

public:
	float			ChargedTime;

protected:

	int				m_shotMax;
	float			m_TurnSpeed;				//��]���x
	float			m_MoveSpeed;				//�ړ����x
	enMoveState		m_MoveState;				//�ړ����
	enShotType		m_ShotType;					//�e�̎��
	enShotNumber	m_ShotNumber;				//�e�̐�

	float			m_Cadence;					//�A�ˑ��x
	float			m_CadenceTimer;				//�A�˃^�C�}�[	
	float			m_NWaySrpeadDeg; 	//NWay�e�̍L����p�x
	float			m_ChargedShotTime;	//�`���[�W�V���b�g����
	const float		m_ChargedShotMax = 5.0f;	//�`���[�W�V���b�g�ő厞��

	bool			m_IsBomb;

private:

	void HandleInput();

	bool m_IsCharging;
	bool m_WasFireDown;
};